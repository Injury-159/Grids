#if defined _inc_grids_
	#endinput
#endif 

#define _inc_grids_	 

#define MAX_GRIDS 					3
#define MAX_GRID_ROWS					10
#define MAX_GRID_COLUMNS				10
#define INVALID_GRID_VECTOR				Vector:INVALID


const INVALID_GRID_DATA 				(cellmin - 1)
	

static Iterator:Grids<MAX_GRIDS>;
static grid_Data[MAX_GRIDS][MAX_GRID_ROWS][MAX_GRID_COLUMNS];
static grid_Rows[MAX_GRIDS];
static grid_Columns[MAX_GRIDS];


//------------------------------------------------------------------------------


Grid_Create(rows, columns)
{
	new grid;
	if((grid = Iter_Alloc(Grids)) > -1)
	{
		grid_Rows[grid] = clamp(rows, 1, MAX_GRID_ROWS);
		grid_Columns[grid] = clamp(columns, 1, MAX_GRID_COLUMNS);
		
		memset(grid_Data[grid][0][0], INVALID_GRID_DATA, MAX_GRIDS * MAX_GRID_ROWS * MAX_GRID_COLUMNS);
		return grid;
	}
	return -1;
}



Grid_Destroy(grid)
{
	if(!Iter_Contains(Grids, grid))
		return;

	memset(grid_Data[grid][0][0], INVALID_GRID_DATA, MAX_GRIDS * MAX_GRID_ROWS * MAX_GRID_COLUMNS);
	grid_Rows[grid] = grid_Columns[grid] = 0;

	Iter_Remove(Grids, grid);
}



Grid_SetData(grid, x, y, data)
{
	if(!Grid_IsValidCell(grid, x, y))
		return;
		
	grid_Data[grid][x][y] = data;
}


Grid_IsValidCell(grid, x, y)
{
	if(!Iter_Contains(Grids, grid))
		return 0;

	return ((-1 < x < grid_Rows[grid]) && (-1 < y < grid_Columns[grid]));
}



Grid_HasSpace(grid, sx, sy, sizex, sizey, &Vector:cells = INVALID_GRID_VECTOR)
{
	if(!Grid_IsValidCell(grid, sx, sy))
		return 0;

	if(sizex < 1 || sizey < 1)
		return 0;

	new 
		endx = (sx - (sizex - 1)),
		endy = (sy - (sizey - 1))
	;

	return Internal_HasSpaceAtGrid(grid, sx, sy, sizex, sizey, endx < 0 ? 0 : endx, endy < 0 ? 0 : endy, cells);
}



static Internal_HasSpaceAtGrid(grid, sx, sy, sizex, sizey, endx, endy, &Vector:cells)
{
	new 
		sumx = sx + sizex,
		sumy = sy + sizey;

	
	if(cells != INVALID_GRID_VECTOR)
		VECTOR_clear(cells);	


	for(new b = sy; b < sumy; b++)
	{
		for(new a = sx; a < sumx; a++)
		{
			if(sizex > 1)
			{
				if(a < (sumx - 1) && (grid_Data[grid][a + 1][b] != INVALID_GRID_DATA  || (a + 1) > (grid_Rows[grid] - 1)))
				{
					if((sx - 1) >= endx) return Internal_HasSpaceAtGrid(grid, (sx - 1), b, sizex, sizey, endx, endy, cells);
					else return 0;
				}
			}

			if(sizey > 1)
			{
				if(b < (sumy - 1) && (grid_Data[grid][a][b + 1] != INVALID_GRID_DATA || (b + 1) > (grid_Columns[grid] - 1)))
				{
					if((sy - 1) >= endy) return Internal_HasSpaceAtGrid(grid, a, (sy - 1), sizex, sizey, endx, endy, cells);
					else return 0;
				}
			}

			if(grid_Data[grid][a][b] != INVALID_GRID_DATA)
				return 0;


			if(cells != INVALID_GRID_VECTOR)
			{
				new tmp[2];
				tmp[0] = a;
				tmp[1] = b;
				VECTOR_push_back_arr(cells, tmp);
			}
		}
	}
	return 1;	
}
