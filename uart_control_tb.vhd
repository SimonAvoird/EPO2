library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.math_real.all;

entity uart_control_tb is
end entity;

architecture structural of uart_control_tb is
    component uart_control is
        port (
            clk                         : in std_logic;
            reset                       : in std_logic;
            data_in_uart, data_out      : in std_logic_vector(7 downto 0);
            data_out_uart, data_in      : out std_logic_vector(7 downto 0);
            write,read                  : out std_logic;
            write_in, data_ready, buffer_empty           : in std_logic
        );
    end component uart_control;

    signal clk,reset,write_in,data_ready,buffer_empty, write,read   : std_logic;
    signal data_in,data_in_uart,data_out,data_out_uart              : std_logic_vector(7 downto 0);    

begin
lbl0: uart_control port map ( clk => clk,
        reset => reset,
        data_in_uart => data_in_uart,
        data_out_uart => data_out_uart,
        data_in => data_in,
        data_out => data_out,
        write => write,
        read => read,
        write_in => write_in,
        data_ready => data_ready,
        buffer_empty => buffer_empty
    );

    clk <=      '0' after 0 ns,
                '1' after 10 ns when clk /= '1' else '0' after 10 ns;
    reset <=    '1' after 0 ns,
                '0' after 20 ns;
    data_in_uart <= "10000000" after 20 ns,
                    "01000000" after 220 ns,
                    "11000000" after 420 ns;
    data_out    <=  "10000000" after 100 ns,
                    "01000000" after 300 ns,
                    "11000000" after 500 ns;
    data_ready  <=  '1' after 40 ns,
                    '0' after 60 ns,
                    '1' after 240 ns,
                    '0' after 260 ns,
                    '1' after 440 ns,
                    '0' after 460 ns;
    write_in    <=  '1' after 100 ns,
                    '0' after 120 ns,
                    '1' after 300 ns,
                    '0' after 320 ns,
                    '1' after 500 ns,
                    '0' after 520 ns;
    buffer_empty <= '0';

end architecture;