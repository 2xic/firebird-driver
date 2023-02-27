${FBPATH}/bin/isql -user sysdba -sql_dialect 3  <<EOL
    CREATE DATABASE 'test' PAGE_SIZE 8192 DEFAULT CHARACTER SET ISO8859_1;
    commit;
    CONNECT 'test';
    create user demo password 'demo' grant admin role;
    create table MYDATA (
        id int not null primary key,
        TESTBOOLEAN CHAR(1) DEFAULT 'J'
    );
    INSERT INTO MYDATA (id, TESTBOOLEAN) values (1, 'J');
    commit;
    quit;
EOL
fbguard -daemon
