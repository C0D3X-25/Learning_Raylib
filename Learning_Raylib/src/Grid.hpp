#pragma once

#include "Typedef.hpp"
#include "Globals.hpp"

#include <vector>
#include <memory>

struct Timer;

struct ModifiedCell {
	ModifiedCell(uint16_t pos_x, uint16_t pos_y, bool state)
		: m_pos_x(pos_x), m_pos_y(pos_y), m_state(state) {}
	uint16_t m_pos_x;
	uint16_t m_pos_y;
	bool m_state{ true };
};


class Grid {
	public:
		Grid(void);

		// Draw inside the window the stat of all cells in the grid.
		void drawGrid(void);

		// Update the grid with only the cells who are changing state
		void updateGrid(void);

		void clearGrid(void);

		// Change the state of the cell with the mouse.
		void userChangeCellState(void);

		// Display in console the current state of the grid array.
		void printArrayGrid(void);
	
		// Calculate the next stage of all cells in the array.
		void nextGeneration(void);

		// Create a grid with random cells alive
		void generateRandomNewGrid(uint16_t seed = 0);

		// Timer between generations
		std::unique_ptr<Timer> m_p_timer{ nullptr };

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

		static const uint16_t m_GRID_WIDTH{ gbl::WINDOW_WIDTH / (m_CELL_WIDTH + m_SPACE_BETWEEN_CELLS) };
		static const uint16_t m_GRID_HEIGHT{ gbl::WINDOW_HEIGHT/ (m_CELL_HEIGHT + m_SPACE_BETWEEN_CELLS) };
	
		// Grid holding cells (all dead by default)
		bool m_current_grid[m_GRID_WIDTH][m_GRID_HEIGHT]{ };

		// Vector holding the cells who gonna change state
		std::vector<ModifiedCell> m_modified_cells;
};


