#include "Grid.hpp"
#include "raylib.h"
#include "Config.hpp"
#include "Timer.hpp"

#include <iostream>
#include <random>


Grid::Grid(void) {
	m_p_timer = std::make_unique<Timer>();
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


void Grid::nextGeneration(void) {

	if (!m_modified_cells.empty()) {
		m_modified_cells.clear();
	}

	uint16_t count_cells_alive{ 0 };
	uint16_t total_cells_alive{ 0 };

#if DEBUG
	uint16_t cell{ 0 };
#endif // DEBUG

	for (uint16_t width{ 0 }; width < m_GRID_WIDTH; width++) {
		for (uint16_t height{ 0 }; height < m_GRID_HEIGHT; height++) {

			// Count alive neighbors cells for current cell
			count_cells_alive = 
				getNWCellState(width, height)
				+ getNCellState(width, height)
				+ getNECellState(width, height)
				+ getWCellState(width, height)
				+ getECellState(width, height)
				+ getSWCellState(width, height)
				+ getSCellState(width, height)
				+ getSECellState(width, height);

			// ===== Rules for living cell =====
			//  - Any live cell with fewer than two live neighbours dies, as if by underpopulation
			//  - Any live cell with two or three live neighbours lives on to the next generation
			//  - Any live cell with more than three live neighbours dies, as if by overpopulation
			if (m_current_grid[width][height] == 1) {
				// Cell is gonna die next generation
				if (count_cells_alive < 2 || count_cells_alive > 3) {
					m_modified_cells.emplace_back(ModifiedCell(width, height, false));
				}
			}

			// ===== Rule for dead cell =====
			//	- Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction
			else {
				// Cell is gonna live next generation
				if (count_cells_alive == 3) {
					m_modified_cells.emplace_back(ModifiedCell(width, height, true));
				}
			}

#if DEBUG
			std::cout << "cell: " << cell << " => " << count_cells_alive << '\n';
			cell++;
#endif // DEBUG

			total_cells_alive += count_cells_alive;
			count_cells_alive = 0;
		}
	}

	// Pause in case of a dead grid or grid with cells alive who are static
	if (total_cells_alive == 0 || m_modified_cells.empty()) {
		gbl::is_game_paused = true;
	}

	gbl::nbr_generation++;
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

