#include "Grid.hpp"
#include "raylib.h"
#include <iostream>



void Grid::drawGrid(void) {

	uint16_t pos_x{ m_SPACE_BETWEEN_CELLS };
	uint16_t pos_y{ m_SPACE_BETWEEN_CELLS };

	for (uint16_t width{ 0 }; width < m_GRID_WIDTH; width++) {
		for (uint16_t height{ 0 }; height < m_GRID_HEIGHT; height++) {


			// Cell is Dead
			if (m_grid_cells[width][height] == 0) {
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
}

void Grid::addLivingCell() {

}

void Grid::printArrayGrid(void) {
	for (uint16_t width{ 0 }; width < m_GRID_WIDTH; width++) {
		for (uint16_t height{ 0 }; height < m_GRID_HEIGHT; height++) {
			std::cout << m_grid_cells[width][height] << " ";
		}
	}
	std::cout << std::endl;
}

void Grid::nextStageGrid(void) {

	uint16_t count_cells_alive{ 0 };
	uint16_t cell{ 0 }; // DEBUG

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

			// Rules for living cell
			//  - Any live cell with fewer than two live neighbours dies, as if by underpopulation
			//  - Any live cell with two or three live neighbours lives on to the next generation
			//  - Any live cell with more than three live neighbours dies, as if by overpopulation
			if (m_grid_cells[width][height] == 1) {
				if (count_cells_alive != 2 && count_cells_alive != 3) {
					m_grid_cells[width][height] = 0;
				}
			}

			// Rule for dead cell
			//	- Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction
			else {
				if (count_cells_alive == 3) {
					m_grid_cells[width][height] = 1;
				}
			}

			std::cout << "cell: " << cell << " => " << count_cells_alive << '\n';
			cell++;

			count_cells_alive = 0;
		}
	}
}

bool Grid::getNWCellState(const uint16_t& current_cell_x, const uint16_t& current_cell_y) {

	if (current_cell_x == 0 
		|| current_cell_y == 0
		|| current_cell_x == m_GRID_WIDTH - 1 
		|| current_cell_y == m_GRID_HEIGHT - 1) {

		return 0;
	}
		return m_grid_cells[current_cell_x - 1][current_cell_y + 1];
}

bool Grid::getNCellState(const uint16_t& current_cell_x, const uint16_t& current_cell_y) {

	if (current_cell_x == 0
		|| current_cell_y == 0
		|| current_cell_x == m_GRID_WIDTH - 1
		|| current_cell_y == m_GRID_HEIGHT - 1) {

		return 0;
	}

	return m_grid_cells[current_cell_x][current_cell_y - 1];
}

bool Grid::getNECellState(const uint16_t& current_cell_x, const uint16_t& current_cell_y) {

	if (current_cell_x == 0
		|| current_cell_y == 0
		|| current_cell_x == m_GRID_WIDTH - 1
		|| current_cell_y == m_GRID_HEIGHT - 1) {

		return 0;
	}

	return m_grid_cells[current_cell_x + 1][current_cell_y + 1];
}

bool Grid::getWCellState(const uint16_t& current_cell_x, const uint16_t& current_cell_y) {

	if (current_cell_x == 0
		|| current_cell_y == 0
		|| current_cell_x == m_GRID_WIDTH - 1
		|| current_cell_y == m_GRID_HEIGHT - 1) {

		return 0;
	}

	return m_grid_cells[current_cell_x - 1][current_cell_y];
}

bool Grid::getECellState(const uint16_t& current_cell_x, const uint16_t& current_cell_y) {

	if (current_cell_x == 0
		|| current_cell_y == 0
		|| current_cell_x == m_GRID_WIDTH - 1
		|| current_cell_y == m_GRID_HEIGHT - 1) {

		return 0;
	}

	return m_grid_cells[current_cell_x + 1][current_cell_y];
}

bool Grid::getSWCellState(const uint16_t& current_cell_x, const uint16_t& current_cell_y) {

	if (current_cell_x == 0
		|| current_cell_y == 0
		|| current_cell_x == m_GRID_WIDTH - 1
		|| current_cell_y == m_GRID_HEIGHT - 1) {

		return 0;
	}

	return m_grid_cells[current_cell_x - 1][current_cell_y - 1];
}

bool Grid::getSCellState(const uint16_t& current_cell_x, const uint16_t& current_cell_y) {

	if (current_cell_x == 0
		|| current_cell_y == 0
		|| current_cell_x == m_GRID_WIDTH - 1
		|| current_cell_y == m_GRID_HEIGHT - 1) {

		return 0;
	}

	return m_grid_cells[current_cell_x][current_cell_y + 1];
}

bool Grid::getSECellState(const uint16_t& current_cell_x, const uint16_t& current_cell_y) {

	if (current_cell_x == 0
		|| current_cell_y == 0
		|| current_cell_x == m_GRID_WIDTH - 1
		|| current_cell_y == m_GRID_HEIGHT - 1) {

		return 0;
	}

	return m_grid_cells[current_cell_x + 1][current_cell_y - 1];
}
