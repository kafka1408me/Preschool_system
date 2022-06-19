DROP TABLE IF EXISTS users;

CREATE TABLE users(
	user_id BIGSERIAL NOT NULL PRIMARY KEY,
	user_name TEXT,
	user_role INT,
	salt CHAR(64),
    user_pwd bytea,
	user_login TEXT,
    UNIQUE (user_login)
);


-------------------------------------------------

DROP TABLE IF EXISTS children;

CREATE TABLE children(
	id BIGSERIAL NOT NULL PRIMARY KEY,
	name TEXT,
    age INT,
    parent_id BIGINT,
    teacher_id BIGINT,
    gender INT,
    CHECK(age > 0),
    UNIQUE(parent_id),
    CONSTRAINT parent_id_constraint FOREIGN KEY(parent_id) REFERENCES users(user_id) ON DELETE CASCADE,
    CONSTRAINT teacher_id_constraint FOREIGN KEY(teacher_id) REFERENCES users(user_id) ON DELETE CASCADE
);

-------------------------------------------------

DROP TABLE IF EXISTS children_traffic;

CREATE TABLE children_traffic(
	child_id BIGINT,
    month INT,
    year INT,
    days INT[],
    UNIQUE(child_id, year, month),
    CONSTRAINT id_constraint FOREIGN KEY(child_id) REFERENCES children(id) ON DELETE CASCADE
);


-------------------------------------------------

DROP TABLE IF EXISTS children_marks;

CREATE TABLE children_marks(
	child_id BIGINT,
    mark_time  TIMESTAMP,
    mark_value INT,
    mark_description TEXT,
    UNIQUE(child_id, mark_time, mark_value, mark_description),
    CONSTRAINT id_constraint FOREIGN KEY(child_id) REFERENCES users(user_id) ON DELETE CASCADE
);
