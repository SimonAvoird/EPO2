library IEEE;
use IEEE.std_logic_1164.all;

entity uart_control is
    port (
        clk                         : in std_logic;
        reset                       : in std_logic;
        data_in_uart, data_out      : in std_logic_vector(7 downto 0);
        data_out_uart, data_in      : out std_logic_vector(7 downto 0);
        write,read                  : out std_logic;
        write_in, data_ready, buffer_empty           : in std_logic
    );
end entity uart_control;

architecture arch of uart_control is
begin
process(clk, reset, write_in, data_ready, data_in_uart, data_out)

variable bool_write: std_logic;
begin
    if rising_edge(clk) then
        if reset = '1' then
            write <= '0';
            read <= '0';
            data_out_uart <= "00000000";
            data_in <= "00000000";
        end if;
        if bool_write = '1' then
            write <= '0';
            bool_write := '0';            
        end if;
        if(write_in = '1' and buffer_empty = '0') then
            data_out_uart <= data_out;
            write <= '1';
            bool_write := '1';
        end if;
        if(data_ready = '1') then
            data_in <= data_in_uart;
            read <= '1';
        elsif(data_ready = '0') then
            read <= '0';
        end if;
    end if;
end process;   

end architecture arch;