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

	global::nbr_generation = 0;
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


void Grid::printArrayGrid(void) const {

	for (uint16_t width{ 0 }; width < m_GRID_WIDTH; width++) {
		for (uint16_t height{ 0 }; height < m_GRID_HEIGHT; height++) {
			std::cout << m_current_grid[width][height] << " ";
		}
	}
	std::cout << '\n';
}


void Grid::allGeneration(void) {

	if (global::nbr_generation > 0) {
		std::cout << "Next generation\n";
		nextGeneration();
	}
	else {
		std::cout << "First generation\n";
		firstGeneration();
	}

	pauseWhenDeadGrid();
	global::nbr_generation++;
}


void Grid::displayDuplicateCells(void) const {

	for (const CellToCheck& cell : m_cells_to_check) {
		uint16_t count{ 0 };

		for (uint16_t i{ 0 }; i < m_cells_to_check.size(); i++) {
			if (cell.m_pos_x == m_cells_to_check[i].m_pos_x 
				&& cell.m_pos_y == m_cells_to_check[i].m_pos_y) {
				count++;
			}
		}

		if (count > 1) {
			std::cout << "cell:" << " [" << cell.m_pos_x << " x " << cell.m_pos_y << "] duplicate: " << count << " time\n";
		}
	}
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

#if DEBUG
	displayDuplicateCells();
#endif // DEBUG


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
		global::game_paused = true;
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

	// Move the cells who changed this generation to a temp vector
	std::vector<CellToCheck> list_cells_this_generation{ std::move(m_cells_to_check) };

#if DEBUG
	std::cout << "Nbr of cells this generation: " << list_cells_this_generation.size() << '\n';
#endif // DEBUG

	for (CellToCheck& cell_this_generation : list_cells_this_generation) {
	
		// Ignore the border
		if (cell_this_generation.m_pos_x == 0
			|| cell_this_generation.m_pos_y == 0
			|| cell_this_generation.m_pos_x == m_GRID_WIDTH - 1
			|| cell_this_generation.m_pos_y == m_GRID_HEIGHT - 1) {

			continue;
		}

		if (!m_cells_to_check.empty()) {
			continue;
		}

		// Empty dynamic array OR the cell is alone
		addNWCell(cell_this_generation);
		addNCell(cell_this_generation);
		addNECell(cell_this_generation);
		addWCell(cell_this_generation);
		addSelfCell(cell_this_generation);
		addECell(cell_this_generation);
		addSWCell(cell_this_generation);
		addSCell(cell_this_generation);
		addSECell(cell_this_generation);

		// TODO: Add the others check here
	}	
}


void Grid::checkSameColumnLastElement(const CellToCheck& current_cell) {

	// At least 1 element in the dynamic array
	if (m_cells_to_check.size() < 1) {
		return;
	}

	if (m_cells_to_check.at(m_cells_to_check.size()).m_pos_x == current_cell.m_pos_x 
		&& m_cells_to_check.at(m_cells_to_check.size()).m_pos_y == current_cell.m_pos_y - 1) {

		addSWCell(current_cell);
		addSCell(current_cell);
		addSECell(current_cell);
	}
}


void Grid::checkSameColumnBeforeLastElement(const CellToCheck& current_cell) {

	// At least 2 elements in the dynamic array
	if (m_cells_to_check.size() < 2) {
		return;
	}

	if (m_cells_to_check.at(m_cells_to_check.size() - 1).m_pos_x == current_cell.m_pos_x
		&& m_cells_to_check.at(m_cells_to_check.size() - 1).m_pos_y == current_cell.m_pos_y - 2) {

		addWCell(current_cell);
		addSelfCell(current_cell);
		addECell(current_cell);
		addSWCell(current_cell);
		addSCell(current_cell);
		addSECell(current_cell);
	}
}


void Grid::checkLastColumn(const CellToCheck& current_cell) {
	
	// At least 2 elements in the dynamic array
	if (m_cells_to_check.size() < 2) {
		return;
	}

	// Calculate the min index for the reverse loop
	const uint16_t RANGE_MIN = global::TOTAL_CELLS_Y_AXIS > m_cells_to_check.size()
		? global::TOTAL_CELLS_Y_AXIS + 3
		: 0; 

	// Reverse loop
	for (uint16_t i{ (uint16_t)m_cells_to_check.size() }; i > RANGE_MIN; i--) {

		// -1X, +2Y from current_cell
		if (m_cells_to_check[i].m_pos_x == current_cell.m_pos_x - 1
			&& m_cells_to_check[i].m_pos_y == current_cell.m_pos_y + 2) {


			continue;
		}
		// -1X, +1Y from current_cell
		if (m_cells_to_check[i].m_pos_x == current_cell.m_pos_x - 1
			&& m_cells_to_check[i].m_pos_y == current_cell.m_pos_y + 1) {


			continue;
		}
		// -1X, +0Y from current_cell
		if (m_cells_to_check[i].m_pos_x == current_cell.m_pos_x - 1
			&& m_cells_to_check[i].m_pos_y == current_cell.m_pos_y) {


			continue;
		}
		// -1X, -1Y from current_cell
		if (m_cells_to_check[i].m_pos_x == current_cell.m_pos_x - 1
			&& m_cells_to_check[i].m_pos_y == current_cell.m_pos_y - 1) {


			continue;
		}
		// -1X, -2Y from current_cell
		if (m_cells_to_check[i].m_pos_x == current_cell.m_pos_x - 1
			&& m_cells_to_check[i].m_pos_y == current_cell.m_pos_y - 2) {


			continue;
		}
	}
}


void Grid::checkBeforeLastColumn(const CellToCheck& current_cell) {

	// At least 2 elements in the dynamic array
	if (m_cells_to_check.size() < 2) {
		return;
	}

	// Calculate the min index for the reverse loop
	const uint16_t MIN_RANGE_INDEX = global::TOTAL_CELLS_Y_AXIS > m_cells_to_check.size()
		? (global::TOTAL_CELLS_Y_AXIS * 2) + 3
		: 0;

	// Reverse loop
	for (uint16_t i{ (uint16_t)m_cells_to_check.size() }; i > MIN_RANGE_INDEX; i--) {

		// -2X, +2Y from current_cell
		if (m_cells_to_check[i].m_pos_x == current_cell.m_pos_x - 2
			&& m_cells_to_check[i].m_pos_y == current_cell.m_pos_y + 2) {


			continue;
		}
		// -2X, +1Y from current_cell
		if (m_cells_to_check[i].m_pos_x == current_cell.m_pos_x - 2
			&& m_cells_to_check[i].m_pos_y == current_cell.m_pos_y + 1) {


			continue;
		}
		// -2X, +0Y from current_cell
		if (m_cells_to_check[i].m_pos_x == current_cell.m_pos_x - 2
			&& m_cells_to_check[i].m_pos_y == current_cell.m_pos_y) {


			continue;
		}
		// -2X, -1Y from current_cell
		if (m_cells_to_check[i].m_pos_x == current_cell.m_pos_x - 2
			&& m_cells_to_check[i].m_pos_y == current_cell.m_pos_y - 1) {


			continue;
		}
		// -2X, -2Y from current_cell
		if (m_cells_to_check[i].m_pos_x == current_cell.m_pos_x - 2
			&& m_cells_to_check[i].m_pos_y == current_cell.m_pos_y - 2) {


			continue;
		}
	}
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

			if (distrib(gen) <= global::CHANCE_TO_GENERATE_LIVING_CELL) {
				m_current_grid[width][height] = 1;
			}
		}
	}
}


bool Grid::getNWCellState(const uint16_t current_cell_x, const uint16_t current_cell_y) const {

	if (current_cell_x == 0 
		|| current_cell_y == 0
		|| current_cell_x == m_GRID_WIDTH - 1 
		|| current_cell_y == m_GRID_HEIGHT - 1) {

		return 0;
	}
	return m_current_grid[current_cell_x - 1][current_cell_y - 1];
}


bool Grid::getNCellState(const uint16_t current_cell_x, const uint16_t current_cell_y) const {

	if (current_cell_x == 0
		|| current_cell_y == 0
		|| current_cell_x == m_GRID_WIDTH - 1
		|| current_cell_y == m_GRID_HEIGHT - 1) {

		return 0;
	}
	return m_current_grid[current_cell_x][current_cell_y - 1];
}


bool Grid::getNECellState(const uint16_t current_cell_x, const uint16_t current_cell_y) const {

	if (current_cell_x == 0
		|| current_cell_y == 0
		|| current_cell_x == m_GRID_WIDTH - 1
		|| current_cell_y == m_GRID_HEIGHT - 1) {

		return 0;
	}
	return m_current_grid[current_cell_x + 1][current_cell_y - 1];
}


bool Grid::getWCellState(const uint16_t current_cell_x, const uint16_t current_cell_y) const {

	if (current_cell_x == 0
		|| current_cell_y == 0
		|| current_cell_x == m_GRID_WIDTH - 1
		|| current_cell_y == m_GRID_HEIGHT - 1) {

		return 0;
	}
	return m_current_grid[current_cell_x - 1][current_cell_y];
}


bool Grid::getECellState(const uint16_t current_cell_x, const uint16_t current_cell_y) const {

	if (current_cell_x == 0
		|| current_cell_y == 0
		|| current_cell_x == m_GRID_WIDTH - 1
		|| current_cell_y == m_GRID_HEIGHT - 1) {

		return 0;
	}
	return m_current_grid[current_cell_x + 1][current_cell_y];
}


bool Grid::getSWCellState(const uint16_t current_cell_x, const uint16_t current_cell_y) const {

	if (current_cell_x == 0
		|| current_cell_y == 0
		|| current_cell_x == m_GRID_WIDTH - 1
		|| current_cell_y == m_GRID_HEIGHT - 1) {

		return 0;
	}
	return m_current_grid[current_cell_x - 1][current_cell_y + 1];
}


bool Grid::getSCellState(const uint16_t current_cell_x, const uint16_t current_cell_y) const {

	if (current_cell_x == 0
		|| current_cell_y == 0
		|| current_cell_x == m_GRID_WIDTH - 1
		|| current_cell_y == m_GRID_HEIGHT - 1) {

		return 0;
	}
	return m_current_grid[current_cell_x][current_cell_y + 1];
}


bool Grid::getSECellState(const uint16_t current_cell_x, const uint16_t current_cell_y) const {

	if (current_cell_x == 0
		|| current_cell_y == 0
		|| current_cell_x == m_GRID_WIDTH - 1
		|| current_cell_y == m_GRID_HEIGHT - 1) {

		return 0;
	}
	return m_current_grid[current_cell_x + 1][current_cell_y + 1];
}


void Grid::addNWCell(const CellToCheck& current_cell) {
	// NW
	m_cells_to_check.emplace_back(CellToCheck(
		current_cell.m_pos_x - 1, current_cell.m_pos_y - 1,
		m_current_grid[current_cell.m_pos_x - 1][current_cell.m_pos_y - 1]));
}

void Grid::addNCell(const CellToCheck& current_cell) {
	// N
	m_cells_to_check.emplace_back(CellToCheck(
		current_cell.m_pos_x, current_cell.m_pos_y - 1,
		m_current_grid[current_cell.m_pos_x][current_cell.m_pos_y - 1]));
}

void Grid::addNECell(const CellToCheck& current_cell) {
	// NE
	m_cells_to_check.emplace_back(CellToCheck(
		current_cell.m_pos_x, current_cell.m_pos_y + 1,
		m_current_grid[current_cell.m_pos_x][current_cell.m_pos_y + 1]));
}

void Grid::addWCell(const CellToCheck& current_cell) {
	// W
	m_cells_to_check.emplace_back(CellToCheck(
		current_cell.m_pos_x - 1, current_cell.m_pos_y,
		m_current_grid[current_cell.m_pos_x - 1][current_cell.m_pos_y]));
}

void Grid::addSelfCell(const CellToCheck& current_cell) {
	// W
	m_cells_to_check.emplace_back(CellToCheck(
		current_cell.m_pos_x, current_cell.m_pos_y,
		m_current_grid[current_cell.m_pos_x][current_cell.m_pos_y]));
}

void Grid::addECell(const CellToCheck& current_cell) {
	// E
	m_cells_to_check.emplace_back(CellToCheck(
		current_cell.m_pos_x + 1, current_cell.m_pos_y,
		m_current_grid[current_cell.m_pos_x + 1][current_cell.m_pos_y]));
}

void Grid::addSWCell(const CellToCheck& current_cell) {
	// SW
	m_cells_to_check.emplace_back(CellToCheck(
		current_cell.m_pos_x - 1, current_cell.m_pos_y + 1,
		m_current_grid[current_cell.m_pos_x - 1][current_cell.m_pos_y + 1]));
}

void Grid::addSCell(const CellToCheck& current_cell) {
	// S
	m_cells_to_check.emplace_back(CellToCheck(
		current_cell.m_pos_x, current_cell.m_pos_y + 1,
		m_current_grid[current_cell.m_pos_x][current_cell.m_pos_y + 1]));
}

void Grid::addSECell(const CellToCheck& current_cell) {
	// SE
	m_cells_to_check.emplace_back(CellToCheck(
		current_cell.m_pos_x + 1, current_cell.m_pos_y + 1,
		m_current_grid[current_cell.m_pos_x + 1][current_cell.m_pos_y + 1]));
}


