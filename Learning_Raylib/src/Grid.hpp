#pragma once
#include "Typedef.hpp"

class Grid {
	public:
		Grid(void) {}

		// Draw inside the window the stat of all cells in the grid.
		void drawGrid(void);

		// Add a linving cell at the given coordinates.
		void addLivingCell(); // TODO 

		// Display in console the current state of the grid array.
		void printArrayGrid(void);
	
		// Calculate the next stage of all cells in the array.
		void nextStageGrid(void);
	
	private:

		// Return the state of the cell at the given coordinates.
		bool getNWCellState(const uint16_t& current_cell_x, const uint16_t& current_cell_y);
		bool getNCellState(const uint16_t& current_cell_x, const uint16_t& current_cell_y);
		bool getNECellState(const uint16_t& current_cell_x, const uint16_t& current_cell_y);

		bool getWCellState(const uint16_t& current_cell_x, const uint16_t& current_cell_y);
		bool getECellState(const uint16_t& current_cell_x, const uint16_t& current_cell_y);

		bool getSWCellState(const uint16_t& current_cell_x, const uint16_t& current_cell_y);
		bool getSCellState(const uint16_t& current_cell_x, const uint16_t& current_cell_y);
		bool getSECellState(const uint16_t& current_cell_x, const uint16_t& current_cell_y);

		const uint16_t m_CELL_WIDTH{ 10 };
		const uint16_t m_CELL_HEIGHT{ 10 };
		const uint16_t m_SPACE_BETWEEN_CELLS{ 1 }; // it's *2
	
		static const uint16_t m_GRID_WIDTH{ 70 };
		static const uint16_t m_GRID_HEIGHT{ 70 };
	
		// Grid holding cells (all dead by default)
		bool m_grid_cells[m_GRID_WIDTH][m_GRID_HEIGHT]{ };
};


