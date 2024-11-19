#pragma once

#include "Typedef.hpp"
#include "Globals.hpp"

#include <vector>
#include <memory> // for ptr

struct Timer;

struct CellToCheck {
	CellToCheck(uint16_t pos_x, uint16_t pos_y, bool state/*, bool verified = false*/)
		: m_pos_x(pos_x), m_pos_y(pos_y), m_state(state)/*, m_verified(verified)*/ {}
	uint16_t m_pos_x{ 0 };
	uint16_t m_pos_y{ 0 };
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
		void printArrayGrid(void) const;

		// Generate the state of the cells each generation
		void allGeneration(void);
	
		// Create a grid with random cells alive
		void generateRandomNewGrid(uint16_t seed = 0);

		// Timer between generations
		std::unique_ptr<Timer> m_p_timer{ nullptr };

	private:

		// DEBUG method
		void displayDuplicateCells(void) const;

		// Calculate the next stage of all cells in the array.
		void firstGeneration(void);

		// Calculate the next stage of the last generation changing state cells only
		void nextGeneration(void);

		// Pause in case of a dead grid or grid with cells alive who are static
		void pauseWhenDeadGrid(void);

		// Count alive neighbors cells for given cell
		uint16_t getNbrNeighborCellsAlive(const uint16_t current_cell_x, const uint16_t current_cell_y);

		// Add the neighbors cells of the changing cells to the vector m_cells_to_check
		void addNeighborOfChangingCells(void);

		// Algorithm - add to dynamic array
		void checkSameColumnLastElement(const CellToCheck& current_cell);
		void checkSameColumnBeforeLastElement(const CellToCheck& current_cell);
		void checkLastColumn(const CellToCheck& current_cell);
		void checkBeforeLastColumn(const CellToCheck& current_cell);
		 

		bool getNWCellState(const uint16_t current_cell_x, const uint16_t current_cell_y) const;
		bool getNCellState(const uint16_t current_cell_x, const uint16_t current_cell_y) const;
		bool getNECellState(const uint16_t current_cell_x, const uint16_t current_cell_y) const;
		bool getWCellState(const uint16_t current_cell_x, const uint16_t current_cell_y) const;
		bool getECellState(const uint16_t current_cell_x, const uint16_t current_cell_y) const;
		bool getSWCellState(const uint16_t current_cell_x, const uint16_t current_cell_y) const;
		bool getSCellState(const uint16_t current_cell_x, const uint16_t current_cell_y) const;
		bool getSECellState(const uint16_t current_cell_x, const uint16_t current_cell_y) const;


		void addNWCell(const CellToCheck& current_cell);
		void addNCell(const CellToCheck& current_cell);
		void addNECell(const CellToCheck& current_cell);
		void addWCell(const CellToCheck& current_cell);
		void addSelfCell(const CellToCheck& current_cell);
		void addECell(const CellToCheck& current_cell);
		void addSWCell(const CellToCheck& current_cell);
		void addSCell(const CellToCheck& current_cell);
		void addSECell(const CellToCheck& current_cell);


		static const uint16_t m_CELL_WIDTH{ 10 };
		static const uint16_t m_CELL_HEIGHT{ 10 };
		static const uint16_t m_SPACE_BETWEEN_CELLS{ 1 };

		static const uint16_t m_GRID_WIDTH{ global::WINDOW_WIDTH / (m_CELL_WIDTH + m_SPACE_BETWEEN_CELLS) };
		static const uint16_t m_GRID_HEIGHT{ global::WINDOW_HEIGHT/ (m_CELL_HEIGHT + m_SPACE_BETWEEN_CELLS) };
	
		// Grid holding cells (all dead by default)
		bool m_current_grid[m_GRID_WIDTH][m_GRID_HEIGHT]{ };

		// Vector holding the cells who gonna change state
		std::vector<CellToCheck> m_cells_to_check;
};


