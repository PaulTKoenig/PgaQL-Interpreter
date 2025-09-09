package storage

import (
    "encoding/csv"
    "os"
)

type Store interface {
    Iterator() *RowIterator
}

type CSVStore struct {
    path string
}

func NewCSVStore(path string) *CSVStore {
    return &CSVStore{path: "data/" + path + ".csv"}
}

type RowIterator struct {
    reader *csv.Reader
    headers []string
    row []string
    file *os.File
}

func (c *CSVStore) Iterator() *RowIterator {
    f, _ := os.Open(c.path)
    r := csv.NewReader(f)
    headers, _ := r.Read()
    return &RowIterator{reader: r, headers: headers, file: f}
}

func (it *RowIterator) Next() bool {
    rec, err := it.reader.Read()
    if err != nil {
        it.file.Close()
        return false
    }
    it.row = rec
    return true
}

func (it *RowIterator) Row() map[string]interface{} {
    out := make(map[string]interface{})
    for i, h := range it.headers {
        out[h] = it.row[i]
    }
    return out
}
