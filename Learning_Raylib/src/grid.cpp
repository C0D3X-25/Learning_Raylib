#include "Grid.hpp"
#include "raylib.h"
#include "Config.hpp"
#include "Timer.hpp"

#include <iostream>
#include <random>


Grid::Grid(void) {
	m_p_timer = std::make_unique<Timer>();
	m_cells_to_check.reserve(100);
}


void Grid::drawGrid(void) {

	uint16_t pos_x{ m_SPACE_BETWEEN_CELLS };
	uint16_t pos_y{ m_SPACE_BETWEEN_CELLS };

	for (uint16_t width{ 0 }; width < m_GRID_WIDTH; width++) {
		for (uint16_t height{ 0 }; height < m_GRID_HEIGHT; height++) {

			// CellToCheck is Dead
			if (m_current_grid[width][height] == 0) {
				DrawRectangle(pos_x, pos_y, m_CELL_WIDTH, m_CELL_HEIGHT, GRAY);
			}

			// CellToCheck is Alive
			else {
				DrawRectangle(pos_x, pos_y, m_CELL_WIDTH, m_CELL_HEIGHT, WHITE);
			}
			pos_y += m_CELL_HEIGHT + m_SPACE_BETWEEN_CELLS;
		}
		pos_x += m_CELL_WIDTH + m_SPACE_BETWEEN_CELLS;
		pos_y = m_SPACE_BETWEEN_CELLS;
	}

	updateTimer(m_p_timer);
}


void Grid::updateGrid(void) {   

	for (const CellToCheck& cell : m_cells_to_check) {
		m_current_grid[cell.m_pos_x][cell.m_pos_y] = cell.m_state;
	}
}


void Grid::clearGrid(void) {

	if (!m_cells_to_check.empty()) {
		m_cells_to_check.clear();
	}

	for (uint16_t width{ 0 }; width < m_GRID_WIDTH; width++) {
		for (uint16_t height{ 0 }; height < m_GRID_HEIGHT; height++) {
			m_current_grid[width][height] = 0;
		}
	}

	gbl::nbr_generation = 0;
}


void Grid::userChangeCellState(void) {

	// Return the position of the cell the mouse is overing
	uint16_t mouse_pos_x{ static_cast<uint16_t>(GetMouseX() / (m_CELL_WIDTH + m_SPACE_BETWEEN_CELLS)) };
	uint16_t mouse_pos_y{ static_cast<uint16_t>(GetMouseY() / (m_CELL_HEIGHT + m_SPACE_BETWEEN_CELLS)) };

#if DEBUG
	//std::cout << "Mouse X: " << mouse_pos_x << ", Mouse Y: " << mouse_pos_y << '\n';
#endif // DEBUG

	if (IsMouseButtonDown(0)) {
		m_current_grid[mouse_pos_x][mouse_pos_y] = 1;
	}
	if (IsMouseButtonDown(1)) {
		m_current_grid[mouse_pos_x][mouse_pos_y] = 0;
	}
}


void Grid::printArrayGrid(void) {

	for (uint16_t width{ 0 }; width < m_GRID_WIDTH; width++) {
		for (uint16_t height{ 0 }; height < m_GRID_HEIGHT; height++) {
			std::cout << m_current_grid[width][height] << " ";
		}
	}
	std::cout << '\n';
}


void Grid::allGeneration(void) {

	if (gbl::nbr_generation > 0) {
		std::cout << "Next generation\n";
		nextGeneration();
	}
	else {
		std::cout << "First generation\n";
		firstGeneration();
	}

	pauseWhenDeadGrid();
	gbl::nbr_generation++;
}


void Grid::firstGeneration(void) {

	if (!m_cells_to_check.empty()) {
		m_cells_to_check.clear();
	}

#if DEBUG
	uint16_t cell_count{ 0 };
#endif // DEBUG

	for (uint16_t width{ 0 }; width < m_GRID_WIDTH; width++) {
		for (uint16_t height{ 0 }; height < m_GRID_HEIGHT; height++) {

			uint16_t nbr_neighbor_cells_alive{ 0 };

			nbr_neighbor_cells_alive = getNbrNeighborCellsAlive(width, height);

			// ===== Rules for living cell =====
			//  - Any live cell with fewer than two live neighbours dies, as if by underpopulation
			//  - Any live cell with two or three live neighbours lives on to the next generation
			//  - Any live cell with more than three live neighbours dies, as if by overpopulation
			if (m_current_grid[width][height] == 1) {
				// CellToCheck is gonna die next generation
				if (nbr_neighbor_cells_alive < 2 || nbr_neighbor_cells_alive > 3) {
					m_cells_to_check.emplace_back(CellToCheck(width, height, false));
				}
			}

			// ===== Rule for dead cell =====
			//	- Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction
			else {
				// CellToCheck is gonna live next generation
				if (nbr_neighbor_cells_alive == 3) {
					m_cells_to_check.emplace_back(CellToCheck(width, height, true));
				}
			}
#if DEBUG
			std::cout << "cell:" << cell_count << " [" << width << " x " << height << "] => " << nbr_neighbor_cells_alive << '\n';
			cell_count++;
#endif // DEBUG

		}
	}
}


void Grid::nextGeneration(void) {

	addNeighborOfChangingCells();

	std::vector<CellToCheck> temp_modified_cells;
	temp_modified_cells.reserve(25);

	uint16_t cell_count{ 0 };

	for (const CellToCheck& checking_cell : m_cells_to_check) {

		uint16_t nbr_cells_alive{ 0 };

		// Count alive neighbors cells for current cell
		nbr_cells_alive = getNbrNeighborCellsAlive(checking_cell.m_pos_x, checking_cell.m_pos_y);

		// ===== Rules for living cell =====
		//  - Any live cell with fewer than two live neighbours dies, as if by underpopulation
		//  - Any live cell with two or three live neighbours lives on to the next generation
		//  - Any live cell with more than three live neighbours dies, as if by overpopulation
		if (m_current_grid[checking_cell.m_pos_x][checking_cell.m_pos_y] == 1) {

			// CellToCheck is gonna die next generation
			if (nbr_cells_alive < 2 || nbr_cells_alive > 3) {
				// Continue if already in Dead state
				if (m_current_grid[checking_cell.m_pos_x][checking_cell.m_pos_y] == 0) {
					continue;
				}
				temp_modified_cells.emplace_back(CellToCheck(checking_cell.m_pos_x, checking_cell.m_pos_y, false));
			}
		}

		// ===== Rule for dead cell =====
		//	- Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction
		else {

			// CellToCheck is gonna live next generation
			if (nbr_cells_alive == 3) {
				// Continue if already in Alive state
				if (m_current_grid[checking_cell.m_pos_x][checking_cell.m_pos_y] == 1) {
					continue;
				}
				temp_modified_cells.emplace_back(CellToCheck(checking_cell.m_pos_x, checking_cell.m_pos_y, true));
			}
		}

#if DEBUG
		std::cout << "cell:" << cell_count << " [" << checking_cell.m_pos_x << " x " << checking_cell.m_pos_y << "] => " << nbr_cells_alive << '\n';
		cell_count++;
#endif // DEBUG

	}

	if (!m_cells_to_check.empty()) {
		m_cells_to_check.clear();
	}

	m_cells_to_check = std::move(temp_modified_cells);
}


void Grid::pauseWhenDeadGrid(void) {
	if (m_cells_to_check.empty()) {
		gbl::game_paused = true;
	}
}


uint16_t Grid::getNbrNeighborCellsAlive(const uint16_t current_cell_x, const uint16_t current_cell_y) {
	
	return (uint16_t)getNWCellState(current_cell_x, current_cell_y)
		+ getNCellState(current_cell_x, current_cell_y)
		+ getNECellState(current_cell_x, current_cell_y)
		+ getWCellState(current_cell_x, current_cell_y)
		+ getECellState(current_cell_x, current_cell_y)
		+ getSWCellState(current_cell_x, current_cell_y)
		+ getSCellState(current_cell_x, current_cell_y)
		+ getSECellState(current_cell_x, current_cell_y);
}


void Grid::addNeighborOfChangingCells(void) {

	std::vector<CellToCheck> neighbor_of_modified_cells;
	neighbor_of_modified_cells.reserve(25);

	std::cout << "Size of cells_to_check vector: " << m_cells_to_check.size() << '\n';

	for (CellToCheck& checking_cell : m_cells_to_check) {
	
		if (checking_cell.m_pos_x == 0
			|| checking_cell.m_pos_y == 0
			|| checking_cell.m_pos_x == m_GRID_WIDTH - 1
			|| checking_cell.m_pos_y == m_GRID_HEIGHT - 1
			/*|| checking_cell.m_verified == true*/) {

			continue;
		}
		//checking_cell.m_verified = true;



		// NW
		neighbor_of_modified_cells.emplace_back(CellToCheck(
			checking_cell.m_pos_x - 1, checking_cell.m_pos_y - 1,
			m_current_grid[checking_cell.m_pos_x - 1][checking_cell.m_pos_y - 1]));
		// N
		neighbor_of_modified_cells.emplace_back(CellToCheck(
			checking_cell.m_pos_x, checking_cell.m_pos_y - 1,
			m_current_grid[checking_cell.m_pos_x][checking_cell.m_pos_y - 1]));
		// NE
		neighbor_of_modified_cells.emplace_back(CellToCheck(
			checking_cell.m_pos_x, checking_cell.m_pos_y + 1,
			m_current_grid[checking_cell.m_pos_x][checking_cell.m_pos_y + 1]));
		// W
		neighbor_of_modified_cells.emplace_back(CellToCheck(
			checking_cell.m_pos_x - 1, checking_cell.m_pos_y,
			m_current_grid[checking_cell.m_pos_x - 1][checking_cell.m_pos_y]));
		// E
		neighbor_of_modified_cells.emplace_back(CellToCheck(
			checking_cell.m_pos_x + 1, checking_cell.m_pos_y,
			m_current_grid[checking_cell.m_pos_x + 1][checking_cell.m_pos_y]));
		// SW
		neighbor_of_modified_cells.emplace_back(CellToCheck(
			checking_cell.m_pos_x - 1, checking_cell.m_pos_y + 1,
			m_current_grid[checking_cell.m_pos_x - 1][checking_cell.m_pos_y + 1]));
		// S
		neighbor_of_modified_cells.emplace_back(CellToCheck(
			checking_cell.m_pos_x, checking_cell.m_pos_y + 1,
			m_current_grid[checking_cell.m_pos_x][checking_cell.m_pos_y + 1]));
		// SE
		neighbor_of_modified_cells.emplace_back(CellToCheck(
			checking_cell.m_pos_x + 1, checking_cell.m_pos_y + 1,
			m_current_grid[checking_cell.m_pos_x + 1][checking_cell.m_pos_y + 1]));

		std::cout << "Size of the neighbor vector: " << neighbor_of_modified_cells.size()
			<< " For cell [" << checking_cell.m_pos_x << " x " << checking_cell.m_pos_y << "]"
			<< '\n';
	}	
	
	m_cells_to_check.insert(m_cells_to_check.end(), neighbor_of_modified_cells.begin(), neighbor_of_modified_cells.end());
}


void Grid::generateRandomNewGrid(uint16_t seed) {

	clearGrid();

	const uint16_t MIN_ROLL{ 1 };
	const uint16_t MAX_ROLL{ 100 };

	// Initialize a random number generator
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(MIN_ROLL, MAX_ROLL);

	for (uint16_t width{ 0 }; width < m_GRID_WIDTH; width++) {
		for (uint16_t height{ 0 }; height < m_GRID_HEIGHT; height++) {

			if (distrib(gen) <= gbl::CHANCE_TO_GENERATE_LIVING_CELL) {
				m_current_grid[width][height] = 1;
			}
		}
	}
}


bool Grid::getNWCellState(const uint16_t current_cell_x, const uint16_t current_cell_y) {

	if (current_cell_x == 0 
		|| current_cell_y == 0
		|| current_cell_x == m_GRID_WIDTH - 1 
		|| current_cell_y == m_GRID_HEIGHT - 1) {

		return 0;
	}
	return m_current_grid[current_cell_x - 1][current_cell_y - 1];
}


bool Grid::getNCellState(const uint16_t current_cell_x, const uint16_t current_cell_y) {

	if (current_cell_x == 0
		|| current_cell_y == 0
		|| current_cell_x == m_GRID_WIDTH - 1
		|| current_cell_y == m_GRID_HEIGHT - 1) {

		return 0;
	}
	return m_current_grid[current_cell_x][current_cell_y - 1];
}


bool Grid::getNECellState(const uint16_t current_cell_x, const uint16_t current_cell_y) {

	if (current_cell_x == 0
		|| current_cell_y == 0
		|| current_cell_x == m_GRID_WIDTH - 1
		|| current_cell_y == m_GRID_HEIGHT - 1) {

		return 0;
	}
	return m_current_grid[current_cell_x + 1][current_cell_y - 1];
}


bool Grid::getWCellState(const uint16_t current_cell_x, const uint16_t current_cell_y) {

	if (current_cell_x == 0
		|| current_cell_y == 0
		|| current_cell_x == m_GRID_WIDTH - 1
		|| current_cell_y == m_GRID_HEIGHT - 1) {

		return 0;
	}
	return m_current_grid[current_cell_x - 1][current_cell_y];
}


bool Grid::getECellState(const uint16_t current_cell_x, const uint16_t current_cell_y) {

	if (current_cell_x == 0
		|| current_cell_y == 0
		|| current_cell_x == m_GRID_WIDTH - 1
		|| current_cell_y == m_GRID_HEIGHT - 1) {

		return 0;
	}
	return m_current_grid[current_cell_x + 1][current_cell_y];
}


bool Grid::getSWCellState(const uint16_t current_cell_x, const uint16_t current_cell_y) {

	if (current_cell_x == 0
		|| current_cell_y == 0
		|| current_cell_x == m_GRID_WIDTH - 1
		|| current_cell_y == m_GRID_HEIGHT - 1) {

		return 0;
	}
	return m_current_grid[current_cell_x - 1][current_cell_y + 1];
}


bool Grid::getSCellState(const uint16_t current_cell_x, const uint16_t current_cell_y) {

	if (current_cell_x == 0
		|| current_cell_y == 0
		|| current_cell_x == m_GRID_WIDTH - 1
		|| current_cell_y == m_GRID_HEIGHT - 1) {

		return 0;
	}
	return m_current_grid[current_cell_x][current_cell_y + 1];
}


bool Grid::getSECellState(const uint16_t current_cell_x, const uint16_t current_cell_y) {

	if (current_cell_x == 0
		|| current_cell_y == 0
		|| current_cell_x == m_GRID_WIDTH - 1
		|| current_cell_y == m_GRID_HEIGHT - 1) {

		return 0;
	}
	return m_current_grid[current_cell_x + 1][current_cell_y + 1];
}

