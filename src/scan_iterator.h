/*
 * This file and its contents are licensed under the Apache License 2.0.
 * Please see the included NOTICE for copyright information and
 * LICENSE-APACHE for a copy of the license.
 */
#ifndef TIMESCALEDB_SCAN_ITERATOR_H
#define TIMESCALEDB_SCAN_ITERATOR_H

#include <postgres.h>
#include <utils/palloc.h>

#include "scanner.h"

#define EMBEDDED_SCAN_KEY_SIZE 5

typedef struct ScanIterator
{
	ScannerCtx ctx;
	TupleInfo *tinfo;
	InternalScannerCtx ictx;
	ScanKeyData scankey[EMBEDDED_SCAN_KEY_SIZE];
} ScanIterator;

#define ts_scan_iterator_create(catalog_table_id, lock_mode, mctx)                                 \
	(ScanIterator)                                                                                 \
	{                                                                                              \
		.ctx = {                                                                                   \
			.table = catalog_get_table_id(ts_catalog_get(), catalog_table_id),                     \
			.nkeys = 0,                                                                            \
			.scandirection = ForwardScanDirection,                                                 \
			.lockmode = lock_mode,                                                                 \
			.result_mctx = mctx,                                                                   \
		}                                                                                          \
	}

static inline TupleInfo *
ts_scan_iterator_tuple_info(const ScanIterator *iterator)
{
	return iterator->tinfo;
}

static inline TupleTableSlot *
ts_scan_iterator_slot(const ScanIterator *iterator)
{
	return iterator->tinfo->slot;
}

static inline HeapTuple
ts_scan_iterator_fetch_heap_tuple(const ScanIterator *iterator, bool materialize, bool *should_free)
{
	return ts_scanner_fetch_heap_tuple(iterator->tinfo, materialize, should_free);
}

static inline TupleDesc
ts_scan_iterator_tupledesc(const ScanIterator *iterator)
{
	return ts_scanner_get_tupledesc(iterator->tinfo);
}

static inline MemoryContext
ts_scan_iterator_get_result_memory_context(const ScanIterator *iterator)
{
	return iterator->tinfo->mctx;
}

static inline void *
ts_scan_iterator_alloc_result(const ScanIterator *iterator, Size size)
{
	return ts_scanner_alloc_result(iterator->tinfo, size);
}

void TSDLLEXPORT ts_scan_iterator_close(ScanIterator *iterator);

void TSDLLEXPORT ts_scan_iterator_scan_key_init(ScanIterator *iterator, AttrNumber attributeNumber,
												StrategyNumber strategy, RegProcedure procedure,
												Datum argument);

/* You must use `ts_scan_iterator_close` if terminating this loop early */
#define ts_scanner_foreach(scan_iterator)                                                          \
	for (ts_scanner_start_scan(&(scan_iterator)->ctx, &(scan_iterator)->ictx);                     \
		 ((scan_iterator)->tinfo =                                                                 \
			  ts_scanner_next(&(scan_iterator)->ctx, &(scan_iterator)->ictx)) != NULL;)

#endif /* TIMESCALEDB_SCAN_ITERATOR_H */
