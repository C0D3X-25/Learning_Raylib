#include "Grid.hpp"
#include "raylib.h"
#include "Config.hpp"
#include "Timer.hpp"

#include <iostream>
#include <random>


Grid::Grid(void) {
	m_p_timer = std::make_unique<Timer>();
	m_modified_cells.reserve(100);
	m_cells_to_check.reserve(100);
}


void Grid::drawGrid(void) {

	uint16_t pos_x{ m_SPACE_BETWEEN_CELLS };
	uint16_t pos_y{ m_SPACE_BETWEEN_CELLS };

	for (uint16_t width{ 0 }; width < m_GRID_WIDTH; width++) {
		for (uint16_t height{ 0 }; height < m_GRID_HEIGHT; height++) {

			// Cell is Dead
			if (m_current_grid[width][height] == 0) {
				DrawRectangle(pos_x, pos_y, m_CELL_WIDTH, m_CELL_HEIGHT, GRAY);
			}

			// Cell is Alive
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

	for (const ModifiedCell& cell : m_modified_cells) {
		m_current_grid[cell.m_pos_x][cell.m_pos_y] = cell.m_state;
	}
}


void Grid::clearGrid(void) {

	if (!m_modified_cells.empty()) {
		m_modified_cells.clear();
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

	if (!m_modified_cells.empty()) {
		m_modified_cells.clear();
	}

#if DEBUG
	uint16_t cell_count{ 0 };
#endif // DEBUG

	for (uint16_t width{ 0 }; width < m_GRID_WIDTH; width++) {
		for (uint16_t height{ 0 }; height < m_GRID_HEIGHT; height++) {

			uint16_t nbr_cells_alive{ 0 };

			nbr_cells_alive = getNbrNeighborCellsAlive(width, height);

			// ===== Rules for living cell =====
			//  - Any live cell with fewer than two live neighbours dies, as if by underpopulation
			//  - Any live cell with two or three live neighbours lives on to the next generation
			//  - Any live cell with more than three live neighbours dies, as if by overpopulation
			if (m_current_grid[width][height] == 1) {
				// Cell is gonna die next generation
				if (nbr_cells_alive < 2 || nbr_cells_alive > 3) {
					m_modified_cells.emplace_back(ModifiedCell(width, height, false));
				}
			}

			// ===== Rule for dead cell =====
			//	- Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction
			else {
				// Cell is gonna live next generation
				if (nbr_cells_alive == 3) {
					m_modified_cells.emplace_back(ModifiedCell(width, height, true));
				}
			}

#if DEBUG
			std::cout << "cell:" << cell_count << " [" << width << " x " << height << "] => " << nbr_cells_alive << '\n';
			cell_count++;
#endif // DEBUG

		}
	}
}


void Grid::nextGeneration(void) {

	std::vector<ModifiedCell> temp_modified_cells;
	temp_modified_cells.reserve(25);

	uint16_t cell_count{ 0 };

	for (const ModifiedCell& modified_cell : m_modified_cells) {


		uint16_t nbr_cells_alive{ 0 };

		// Count alive neighbors cells for current cell
		nbr_cells_alive = getNbrNeighborCellsAlive(modified_cell.m_pos_x, modified_cell.m_pos_y);

		// ===== Rules for living cell =====
		//  - Any live cell with fewer than two live neighbours dies, as if by underpopulation
		//  - Any live cell with two or three live neighbours lives on to the next generation
		//  - Any live cell with more than three live neighbours dies, as if by overpopulation
		if (m_current_grid[modified_cell.m_pos_x][modified_cell.m_pos_y] == 1) {
			// Cell is gonna die next generation
			if (nbr_cells_alive < 2 || nbr_cells_alive > 3) {
				temp_modified_cells.emplace_back(ModifiedCell(modified_cell.m_pos_x, modified_cell.m_pos_y, false));
			}
		}

		// ===== Rule for dead cell =====
		//	- Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction
		else {
			// Cell is gonna live next generation
			if (nbr_cells_alive == 3) {
				temp_modified_cells.emplace_back(ModifiedCell(modified_cell.m_pos_x, modified_cell.m_pos_y, true));
			}
		}

#if DEBUG
		std::cout << "cell:" << cell_count << " [" << modified_cell.m_pos_x << " x " << modified_cell.m_pos_y << "] => " << nbr_cells_alive << '\n';
		cell_count++;
#endif // DEBUG

	}

	if (!m_modified_cells.empty()) {
		m_modified_cells.clear();
	}

	m_modified_cells = std::move(temp_modified_cells);
}

void Grid::pauseWhenDeadGrid(void) {

	if (m_modified_cells.empty()) {
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


void Grid::addNeighborOfChangingCells(ModifiedCell& modified_cell) {
	
	if (modified_cell.m_pos_x == 0
		|| modified_cell.m_pos_y == 0
		|| modified_cell.m_pos_x == m_GRID_WIDTH - 1
		|| modified_cell.m_pos_y == m_GRID_HEIGHT - 1
		|| modified_cell.m_verified == true) {

		return;
	}

	modified_cell.m_verified = true;

	m_cells_to_check.emplace_back(ModifiedCell(
		modified_cell.m_pos_x - 1, modified_cell.m_pos_y - 1,
		m_current_grid[modified_cell.m_pos_x - 1][modified_cell.m_pos_y - 1], true));
			
	m_cells_to_check.emplace_back(ModifiedCell(
		modified_cell.m_pos_x - 1, modified_cell.m_pos_y - 1,
		m_current_grid[modified_cell.m_pos_x - 1][modified_cell.m_pos_y - 1], true));

	m_cells_to_check.emplace_back(ModifiedCell(
		modified_cell.m_pos_x - 1, modified_cell.m_pos_y - 1,
		m_current_grid[modified_cell.m_pos_x - 1][modified_cell.m_pos_y - 1], true));

	m_cells_to_check.emplace_back(ModifiedCell(
		modified_cell.m_pos_x - 1, modified_cell.m_pos_y - 1,
		m_current_grid[modified_cell.m_pos_x - 1][modified_cell.m_pos_y - 1], true));

	m_cells_to_check.emplace_back(ModifiedCell(
		modified_cell.m_pos_x - 1, modified_cell.m_pos_y - 1,
		m_current_grid[modified_cell.m_pos_x - 1][modified_cell.m_pos_y - 1], true));
	

	m_modified_cells.push_back(); // ?
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

