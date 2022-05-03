project('UDP_server', 'c', version: '0.1.0')

c_flag = ['-Wall',
          '-Wextra',
          '-Werror',
          '-g3']

includes = include_directories('include')

server_src = ['src/main.c',
              'src/server.c',
              'src/tlv_parser.c',
              'src/commands_proc.c']

executable('server', server_src, c_args: c_flag, include_directories: includes)

client_src = ['src/simple_client.c',
              'src/tlv_parser.c']

executable('client', client_src, c_args: c_flag, include_directories: includes)