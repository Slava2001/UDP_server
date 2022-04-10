project('UDP_server', 'c', version: '0.0.0')

c_flag = ['-Wall',
          '-Wextra',
          '-Werror']

includes = include_directories('include')

src = ['src/main.c',
       'src/server.c']

executable('server', src, c_args: c_flag, include_directories: includes)
executable('client', 'src/simple_client.c', c_args: c_flag, include_directories: includes)