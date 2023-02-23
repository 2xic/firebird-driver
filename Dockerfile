from controlsoft/firebird

RUN apt-get update --fix-missing -y

RUN echo "\
# Firebird generated password for user SYSDBA is:\n\
#\n\
ISC_USER=sysdba\n\
ISC_PASSWORD=7d85568099\n\
#\n\
# Also set legacy variable though it can't be exported directly\n\
#\n\
ISC_PASSWD=7d85568099\n\
#\n\
#\n\
# Your password can be changed to a more suitable one using\n\
# SQL operator ALTER USER.\n\
#\n\
" > /opt/firebird/SYSDBA.password
RUN cat /opt/firebird/SYSDBA.password

RUN apt-get install file -y
RUN echo "\nWireCrypt = Enabled\n" >> ${FBPATH}/firebird.conf

ENV FIREBIRD_PASSWORD=demo

COPY setup_database.sh setup_database.sh
