-- This file and its contents are licensed under the Apache License 2.0.
-- Please see the included NOTICE for copyright information and
-- LICENSE-APACHE for a copy of the license.

-- INSERT data to create a new chunk after update or restore.
INSERT INTO devices(id,floor) VALUES
('dev5', 5);
INSERT INTO "two_Partitions"("timeCustom", device_id, device_id_2, series_0, series_1, series_2) VALUES
(1258894000000000000, 'dev5', 'dev1', 2.2, 1, 2);

