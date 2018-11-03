#define MAX_GRIDS 						1000
#define MAX_GRID_R						10
#define MAX_GRID_C						10
#define INVALID_GRID_VECTOR				Vector:INVALID
#define INVALID_GRID_DATA 				(cellmin - 1)



#define Grid_ClearAllData(%0) \
	memset(grid_Data[(%0)][0][0], INVALID_GRID_DATA, MAX_GRIDS * MAX_GRID_R * MAX_GRID_C)



#define Grid_IsValidCell(%0,%1,%2) \
	((0 <= (%1) < grid_Rows[(%0)]) && (0 <= (%2) < grid_Rows[(%0)]))	



static 
	Iterator:Grids<MAX_GRIDS>,
	grid_Data[MAX_GRIDS][MAX_GRID_R][MAX_GRID_C],
	grid_Rows[MAX_GRIDS],
	grid_Columns[MAX_GRIDS];


// ———————————————————————————————————————————————————————————————————
// FUNCTIONS
// ———————————————————————————————————————————————————————————————————


Grid_Create(rows, columns)
{
	new idx;
	if((idx = Iter_Alloc(Grids)) > -1)
	{
		grid_Rows[idx] = clamp(rows, 1, MAX_GRID_R);
		grid_Columns[idx] = clamp(columns, 1, MAX_GRID_C);

		Grid_ClearAllData(idx);
		return idx;
	}
	return INVALID;
}



Grid_Destroy(index)
{
	if(!Iter_Contains(Grids, index))
		return;

	Grid_ClearAllData(index);
	grid_Rows[index] = grid_Columns[index] = 0;

	Iter_Remove(Grids, index);
}



Grid_SetData(index, cellx, celly, data)
{
	if(!Iter_Contains(Grids, index))
		return;

	if(!Grid_IsValidCell(index, cellx, celly))
		return;


	grid_Data[index][cellx][celly] = data;
}



stock Grid_Clear(index)
{
	if(!Iter_Contains(Grids, index))
		return;

	Grid_ClearAllData(index);		
}



Grid_HasSpace(index, sx, sy, sizex, sizey, &Vector:cells = INVALID_GRID_VECTOR)
{
	if(!Iter_Contains(Grids, index))
		return 0;

	if(sizex < 1 || sizey < 1)
		return 0;


	if(!Grid_IsValidCell(index, sx, sy))
		return 0;

	new 
		endx = (sx - (sizex - 1)),
		endy = (sy - (sizey - 1))
	;

	return Internal_HasSpaceAtGrid(index, sx, sy, sizex, sizey, endx < 0 ? 0 : endx, endy < 0 ? 0 : endy, cells);
}



static Internal_HasSpaceAtGrid(index, sx, sy, sizex, sizey, endx, endy, &Vector:cells)
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
				if(a < (sumx - 1) && (grid_Data[index][a + 1][b] != INVALID_GRID_DATA  || (a + 1) > (grid_Rows[index] - 1)))
				{
					if((sx - 1) >= endx) return Internal_HasSpaceAtGrid(index, (sx - 1), b, sizex, sizey, endx, endy, cells);
					else return 0;
				}
			}

			if(sizey > 1)
			{
				if(b < (sumy - 1) && (grid_Data[index][a][b + 1] != INVALID_GRID_DATA || (b + 1) > (grid_Columns[index] - 1)))
				{
					if((sy - 1) >= endy) return Internal_HasSpaceAtGrid(index, a, (sy - 1), sizex, sizey, endx, endy, cells);
					else return 0;
				}
			}

			if(grid_Data[index][a][b] != INVALID_GRID_DATA)
				return 0;


			if(cells != INVALID_GRID_VECTOR)
			{
				new tmp[2];
				tmp[0] = a;
				tmp[0] = b;
				VECTOR_push_back_arr(cells, tmp);
			}
		}
	}
	return 1;	
}



#undef Grid_ClearAllData
#undef Grid_IsValidCell