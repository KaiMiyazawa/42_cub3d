#include "cub3d.h"

void	draw_floor(t_mlx_info *mlx_info)
{
	size_t	begin_y;
	size_t	end_y;
	size_t	begin_x;
	size_t	end_x;
	int		color;

	begin_y = mlx_info->win_height / 2;
	end_y = mlx_info->win_height;
	end_x = mlx_info->win_width;
	color = 0xB6BFC8;
	while (begin_y < end_y)
	{
		begin_x = 0;
		while (begin_x < end_x)
		{
			my_mlx_pixel_put(&mlx_info->img_data, begin_x, begin_y, color);
			begin_x++;
		}
		begin_y++;
	}
}

void	draw_ceiling(t_mlx_info *mlx_info)
{
	size_t	begin_y;
	size_t	end_y;
	size_t	begin_x;
	size_t	end_x;
	int		color;

	begin_y = 0;
	end_y = mlx_info->win_height / 2;
	end_x = mlx_info->win_width;
	color = 0x3E434C;
	while (begin_y < end_y)
	{
		begin_x = 0;
		while (begin_x < end_x)
		{
			my_mlx_pixel_put(&mlx_info->img_data, begin_x, begin_y, color);
			begin_x++;
		}
		begin_y++;
	}
}

void	ready_to_raycast(t_mlx_info *mlx_info, t_player_info *player_info, size_t i)
{
	player_info->camera_x = 2 / (double)mlx_info->win_width * i - 1;
	player_info->ray_x = player_info->dir_x + player_info->plane_x * player_info->camera_x; // it depends on the player direction.
	player_info->ray_y = player_info->dir_y + player_info->plane_y * player_info->camera_x; // N or S or W or E. in this case, N.
//		printf("ray_x: %f   ray_y: %f\n", player_info->ray_x, player_info->ray_y);
	player_info->map_x = (size_t)player_info->pos_x;
	player_info->map_y = (size_t)player_info->pos_y;
	player_info->delta_x = (player_info->ray_x == 0) ? 1e30 : fabs(1 / player_info->ray_x);
	player_info->delta_y = (player_info->ray_y == 0) ? 1e30 : fabs(1 / player_info->ray_y);
	if (player_info->ray_x < 0)
	{
		player_info->step_x = -1;
		player_info->side_x = (player_info->pos_x - player_info->map_x) * player_info->delta_x;
	}
	else // ray_x >= 0
	{
		player_info->step_x = 1;
		player_info->side_x = ((player_info->map_x + 1) - player_info->pos_x) * player_info->delta_x;
	}
	if (player_info->ray_y < 0)
	{
		player_info->step_y = -1;
		player_info->side_y = (player_info->pos_y - player_info->map_y) * player_info->delta_y;
	}
	else // ray_y >= 0
	{
		player_info->step_y = 1;
		player_info->side_y = ((player_info->map_y + 1) - player_info->pos_y) * player_info->delta_y;
	}
}

void	raycast_until_hit_wall(t_player_info *player_info, t_map_info *map_info)
{
	int		hit;

	hit = 0;
	while (hit == 0)
	{
		if (player_info->side_x < player_info->side_y)
		{
			player_info->side_x += player_info->delta_x;
			player_info->map_x += player_info->step_x;
			if (player_info->step_x == -1)
				player_info->wall_side = WEST;
			else
				player_info->wall_side = EAST;
		}
		else // side_y < side_x
		{
			player_info->side_y += player_info->delta_y;
			player_info->map_y += player_info->step_y;
			if (player_info->step_y == -1)
				player_info->wall_side = NORTH;
			else
				player_info->wall_side = SOUTH;
		}
		if (map_info->map[player_info->map_y][player_info->map_x] == '1')
			hit = 1;
	}
}

size_t	get_wall_height(t_player_info *player_info)
{
	if (player_info->wall_side == NORTH || player_info->wall_side == SOUTH)
		player_info->perpendicular_distance_to_wall = player_info->side_y - player_info->delta_y;
	else // player_info->wall_side == WEST || EAST
		player_info->perpendicular_distance_to_wall = player_info->side_x - player_info->delta_x;
	return ((int)(WALL_SIZE_WEIGHT / player_info->perpendicular_distance_to_wall));
}

int	get_wall_color(int wall_side, t_map_info *map_info)
{
	(void)map_info;
	if (wall_side == NORTH)
		return (0x00FF0000); // red
	else if (wall_side == SOUTH)
		return (0x0000FF00); // pink
	else if (wall_side == WEST)
		return (0x000000FF); // blue
	else // wall_side == EAST
		return (0x00FFFF00); // green
}

void	draw_vertical_line(t_mlx_info *mlx_info, t_player_info *player_info, size_t wall_height, size_t i, int color)
{
	double	wall_x;
	int		tex_x;
	size_t	begin_x;
	size_t	begin_y;
	size_t	current_height;

	if (player_info->wall_side == NORTH || player_info->wall_side == SOUTH)
		wall_x = player_info->pos_y + player_info->perpendicular_distance_to_wall * player_info->ray_y;
	else
		wall_x = player_info->pos_x + player_info->perpendicular_distance_to_wall * player_info->ray_x;
	wall_x -= floor(wall_x);
	tex_x = int(wall_x * double(tex_width));
	if ((player_info->wall_side == NORTH || player_info->wall_side == SOUTH) && player_info->ray_x < 0)
	begin_x = i;
	begin_y = (mlx_info->win_height / 2) - (wall_height / 2);
	current_height = 0;
	while (current_height < wall_height)
	{
		if (begin_y < 0 || begin_y >= mlx_info->win_height)
			begin_y++;
		else
			my_mlx_pixel_put(&mlx_info->img_data, begin_x, begin_y++, color);
		current_height++;
	}
}

void	draw_loop(t_mlx_info *mlx_info, t_map_info *map_info, t_player_info *player_info)
{
	size_t	i;
	size_t	wall_height;
	int		color;

	i = 0;
	draw_floor(mlx_info);
	draw_ceiling(mlx_info);
	while(i < mlx_info->win_width)
	{
		ready_to_raycast(mlx_info, player_info, i);
		raycast_until_hit_wall(player_info, map_info);
		wall_height = get_wall_height(player_info);
		color = get_wall_color(player_info->wall_side, map_info);
		draw_vertical_line(mlx_info, player_info, wall_height, i, color);
		i++;
	}
}
