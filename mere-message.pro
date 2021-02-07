TEMPLATE = subdirs
SUBDIRS = \
        lib     \  # mere-message-lib
        client  \  # mere-message-client
        server  \  # mere-message-server

#
# Show the actual project file(.pro)
#
lib.file = lib/mere-message-lib.pro
client.file = samples/client/client.pro
server.file = samples/server/server.pro


#
# Show the dependencies
#
client.depends = lib
server.depends = lib
