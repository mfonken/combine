----------------------------------------------
-- Package include
----------------------------------------------
-- IEEE packages
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.math_real.ALL;
--use ieee.std_logic_integer.all;

-- Project constants
use work.ora_types.all;
use work.ora_math.all;

----------------------------------------------
-- Kalman Architecture
----------------------------------------------
entity kalman is
	generic
	(
		
		
	);
	port
	(
		-- Global clock
		clock        		: in    	std_logic;
		reset					: in		std_logic;
		ena       			: in    	std_logic;
		busy					: out		std_logic;
		value_in      		: in  	std_logic_vector(7 downto 0);
		velocity_in  		: in 		std_logic_vector(7 downto 0);
		value_out      	: out  	std_logic_vector(7 downto 0);
		velocity_out  		: out		std_logic_vector(7 downto 0);
	);
end kalman;

--------------------------------------------------------------------------------
-- Kalman behaviour
--------------------------------------------------------------------------------
architecture gbehaviour of kalman is
		
		
		constant delta_time				: integer				:= 1;
		
		this			: integer				:= 0;
		vel			: integer				:= 0;
		
		K0				: integer				:= 0;
		K1				: integer				:= 0;
		P00			: integer				:= 0;
		P01			: integer				:= 0;
		P10			: integer				:= 0;
		P11			: integer				:= 0;
		
		rate			: integer				:= 0;
		bias			: integer				:= 0;
		timestamp 	: integer				:= 0;
begin


	kalman_update : process( gclk )
	constant vu				: real				:= 0.001;
	constant bu				: real				:= 0.5;
	constant su				: real				:= 0.7;
	variable dP11			: real				:= 0.0;
	variable	S				: real				:= 0.0;
	variable	delta_value	: real				:= 0.0;
	
	begin
		if reset_n = '0' then	
			rate 	:= 0.0;
			bias	:= 0.0;
			this 	:= 0.0;
			vel 	:= 0.0;
			K0		:= 0.0;
			K1		:= 0.0;
			P00	:= 0.0;
			P01	:= 0.0;
			P10	:= 0.0;
			P11 	:= 0.0;
			timestamp := 0;
		elsif rising_edge(clock) then
			vel := value_in - this;
			rate := velocity_in - bias;
			this := this + ( delta_time * rate );
			dP11	:=	delta_time * P11;
			P00	:= delta_time * ( dP11 - P01 - P10 + VU );
			P01	:= P01 - dP11;
			P10	:= P10 - dP11;
			P11 	:= P11 + ( bu * delta_time );
			S		:=	P00 + su;
			K0		:= P00 / S;
			K1		:= P10 / S;
			delta_value := value_in - this;
			this 	:= this + ( K0 * delta_value );
			bias	:= bias + ( K1 * delta_value );
			P00	:= P00 - ( K0 * P00 );
			P01	:= P01 - ( K0 * P01 );
			P10	:= P10 - ( K1 * P00 );
			P11	:= P11 - ( K1 * P01 );
		end if;
		
		value_out <= this;
		velocity_out <= vel;
	
	end process kalman_update;
	
end gbehaviour;