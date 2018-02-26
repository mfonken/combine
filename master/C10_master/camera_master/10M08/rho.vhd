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


 function density_processor( X : x_array )
            return prediction_t is
	constant VARIANCE_NORMAL := interger := ;
	constant VARIANCE_SCALE := interger := ;
	constant K_TARGET := real := ;
	constant PUNISH_FACTOR := integer := ;
	constant COVERAGE_TARGET := real := ;
	variable P 		: prediction_t;
	variable c 		: integer :=	0;
	variable m 		: integer :=	0;
	variable v 		: integer :=	0;
	variable g 		: integer :=	0;
	variable c1		: integer :=	0;
	variable c2		: integer :=	0;
	variable locc	: integer :=	0;
	variable loc	: array( 1 downto 0 ) of integer range 0 to MAX_VALUE;
	variable denc	: integer :=	0;
	variable den	: array( 1 downto 0 ) of integer range 0 to MAX_VALUE;
	variable maxc	: integer :=	0;
	variable max	: array( 2 downto 0 ) of integer range 0 to MAX_VALUE;
	variable ac		: real	 :=	0.0;
	variable count	: integer :=	0;
	variable M0		: real	 :=	0.0;
	variable M1		: real	 :=	0.0;
	variable has	: std_logic := '0';
  begin
    for i in 0 to ( FRAME_HEIGHT - 1 ) loop
		t := maxX(X);
		Kalman.update(m, 0.0);
		v := VARIANCE_NORMAL * ( 1 + VARIANCE_SCALE * ( K_TARGET - K.K0) );
		
		if v < t then
			g := t - v;
		else
			g := 0;
		end if;
      c1 := X(i);
		if c1 > ( t + 1 ) then
			c1 := t - PUNISH_FACTOR * ( c1 - t );
		end if;
		if c1 < g then
			c2 := 0;
		else
			c2 := c1 - g;
		end if;
		if c2 > 0 then
			has := '1';
			count := count + 1;
			M0 := M0 + ( ( c2 + M0 ) / count );
			M1 := M1 + ( ( ( c2 * i ) + M1 ) / count );
			if c2 > maxc then
				maxc := c2;
			end if;
		elsif has = '1' then
			locc := M1 / M0;
			denc := M0;
			if denc > den(0) then
				loc(1) := loc(0);
				loc(0) := locc;
				den(1) := den(0);
				den(0) := denc;
				max(2) := max(1);
				max(1) := max(0);
				max(0) := maxc;
			elsif denc > den(1) then
				loc(1) := locc;
				den(1) := denc;
				max(2) := max(1);
				max(1) := maxc;
			end if;
			
			QF := QF + M0;
			M0 := 0;
			M1 := 0;
			count := 0;
			has := '0';			
		end if;
    end loop;
	 
	 FT := QF / QT;
	 
	 P.primary_new 	:= loc(0);
	 P.secondary_new 	:= loc(1);
	 
	 P.primary_prob	:= max(0) / v;
	 P.secondary_prob := max(1) / v;
	 
	 ac := 1 - FT / COVERAGE_TARGET;
	 if ac < 0 then
		P.alternate_prob := max(2) / v;
	 else	
		P.alternate_prob := ac;
	end if;
	return M;
end density_mapper;



function prediction_selector( P : prediction_t )
			return prediction_t is
	variable P 		: prediction_t;
	variable Ax		: integer :=	0;
	variable Ay		: integer :=	0;
	variable Bx		: integer :=	0;
	variable By		: integer :=	0;
	variable t		: integer :=	0;

	variable px		: integer :=	0;
	variable p1		: integer :=	0;
	variable p1		: integer :=	0;
	
	begin
		Ax := Px.primary_new;
		Bx := Px.secondary_new;
		Ay := Py.primary_new;
		By := Py.secondary_new;
		
		if Ay > By then
			Ay := Py.secondary_new;
			By := Py.primary_new;
		end if;
		if not ( ( Q(0) > Q(1) ) xor ( Ax > Bx ) ) then
			Ax := Px.secondary_new;
			Bx := Px.primary_new;
		end if;
		
		px := Kxp.this + Kxp.vel;
		p1 := abs(px - Ax);
		p2 := abs(px - Bx);
		
		if p1 > p2 then
			Kxp.update(Bx, 0.0);
			Kxs.update(Ax, 0.0);
			Kyp.update(Ay, 0.0);
			Kys.update(By, 0.0);
			P.selection_type := OPPOSITE;
		else
			Kxp.update(Ax, 0.0);
			Kxs.update(Bx, 0.0);
			Kyp.update(By, 0.0);
			Kys.update(Ay, 0.0);
			P.selection_type := SIMILAR;
		end if;
	return P;
end prediction_selector;





























