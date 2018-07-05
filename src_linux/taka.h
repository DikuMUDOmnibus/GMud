/*******************************************************************************
 *         _               | File Name:   taka.c
 *        / \      _-'     | Description: work by taka for Feltain
 *      _/|  \-''- _ /     |
 * __-' |          \       |
 *     /              \    |
 *     /       "o.  |o |   |
 *     |            \ ;    |
 *                   ',    |
 *        \_         __\   | (c) 2000-2001 TAKA & Fear_Phantom
 *          ''-_    \.//   | (c) 2000-2001 The GhostMud Project Team
 *            / '-____'    |
 *           /             | You may use this code under GNU license restriction
 *         _'  The Wolf    | 1) This header block remains in the code.
 *       _-'   strikes!    | 2) You email me at a_ghost_dancer@excite.com
 *_________________________|    letting me know you are using this code
 *                              please incluse your name, your mud name
 * All rights reserved          your mud address, your email and this file
 * GhostMud is copyrighted      name.
 * by TAKA (c) 2003          3) In your help files mention me where appropriate
 *                              IE: help snippets.
 *********************************************************************************/

/***Change #001 allow areas to flip by a factor of degrees***/

/* by taka for area flip of boats */
#define AREA_BOAT_VNUM 10800

/* rotation value 25% = 1, 50% = 2, 75% = 3*/
#define FLIP_25 1
#define FLIP_50 2
#define FLIP_75 3

/* count to flip */
#define BOAT_COUNT	10

/* prototypes for: change log #001 */
int  rotate_value (int Int, int Factor);
void read_area    (AREA_DATA *pArea, int From, int To);


/***End change #001***/
