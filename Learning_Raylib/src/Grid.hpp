#pragma once

#include "Typedef.hpp"
#include "Globals.hpp"
#include "Config.hpp"
#include "Timer.hpp"


class Grid {
	public:
		Grid(void) {}

		// Draw inside the window the stat of all cells in the grid.
		void drawGrid(void);

		// Change the state of the cell with the mouse.
		void userChangeCellState(void);

		// Display in console the current state of the grid array.
		void printArrayGrid(void);
	
		// Calculate the next stage of all cells in the array.
		void nextGenerationGrid(void);
	

	private:

		// Return the state of the North-West cell of the given cell.
		bool getNWCellState(const uint16_t current_cell_x, const uint16_t current_cell_y);
		// Return the state of the North cell of the given cell.
		bool getNCellState(const uint16_t current_cell_x, const uint16_t current_cell_y);
		// Return the state of the North-East cell of the given cell.
		bool getNECellState(const uint16_t current_cell_x, const uint16_t current_cell_y);
		// Return the state of the West cell of the given cell.
		bool getWCellState(const uint16_t current_cell_x, const uint16_t current_cell_y);
		// Return the state of the East cell of the given cell.
		bool getECellState(const uint16_t current_cell_x, const uint16_t current_cell_y);
		// Return the state of the South-West cell of the given cell.
		bool getSWCellState(const uint16_t current_cell_x, const uint16_t current_cell_y);
		// Return the state of the South cell of the given cell.
		bool getSCellState(const uint16_t current_cell_x, const uint16_t current_cell_y);
		// Return the state of the South-East cell of the given cell.
		bool getSECellState(const uint16_t current_cell_x, const uint16_t current_cell_y);


		static const uint16_t m_CELL_WIDTH{ 10 };
		static const uint16_t m_CELL_HEIGHT{ 10 };
		static const uint16_t m_SPACE_BETWEEN_CELLS{ 1 };

		static const uint16_t m_GRID_WIDTH{ WINDOW_WIDTH / (m_CELL_WIDTH + m_SPACE_BETWEEN_CELLS) };
		static const uint16_t m_GRID_HEIGHT{ WINDOW_HEIGHT/ (m_CELL_HEIGHT + m_SPACE_BETWEEN_CELLS) };
	
		// Grid holding cells (all dead by default)
		bool m_grid_cells[m_GRID_WIDTH][m_GRID_HEIGHT]{ };

		// Timer between generations
		Timer m_timer;

};


