library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

use work.ora_types.all;

package ora_math is
-- Density mapper descriptor
  function density_mapper( F : frame_t )
            return density_map_t;
-- Convolution descriptor
  function convolveX(
              X_l : integer;
              X   : x_array;
              H_l : integer;
              H   : k_array
            )
            return convolve_result_t;
-- Convolution descriptor
  function convolveY(
              X_l : integer;
              X   : y_array;
              H_l : integer;
              H   : k_array
            )
            return convolve_result_t;
-- Maxima detection descriptor
  function maximax(
              X : convolve_result_t
            )
            return x_peaks_a;
	function maximay(
              Y : convolve_result_t
            )
            return y_peaks_a;
end ora_math;

package body ora_math is
-- Density mapper body
  function density_mapper( F : frame_t )
            return density_map_t is
	variable M : density_map_t;
	
  begin
    for i in 0 to ( FRAME_HEIGHT - 1 ) loop
      for j in 0 to ( FRAME_WIDTH - 1 ) loop
        if ( F(i)(j) = '1' ) then
          M.x_map(j) := M.x_map(j) + 1;
          M.y_map(i) := M.y_map(i) + 1;
        end if;
      end loop;
    end loop;
	 return M;
  end density_mapper;


-- Get max X value body
  function maxX(
              X : x_array
            )
            return integer range 0 to MAX_VALUE is
		variable max	 	:	integer range 0 to MAX_VALUE 	 := 0;
	begin
	for i in 1 to ( FRAME_HEIGHT - 1 ) loop
		if X(i) > max then
			max := X(i);
		end if;
	end loop;
	return max;
	end maxX;
	
  -- Get max Y value body
  function maxY(
              Y : y_array
            )
            return integer range 0 to MAX_VALUE is
		variable max	 	:	integer range 0 to MAX_VALUE 	 := 0;
	begin
	for i in 1 to ( FRAME_WIDTH - 1 ) loop
		if Y(i) > max then
			max := X(i);
		end if;
	end loop;
	return max;
	end maxX;
end ora_math;
