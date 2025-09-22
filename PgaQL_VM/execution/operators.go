package execution

import (
    "github.com/PaulTKoenig/PgaQL_Backend/storage"
)

type Row map[string]interface{}

type Operator interface {
	Next() (Row, bool)
}

type Scan struct {
	iter *storage.RowIterator
}

func (s *Scan) Next() (Row, bool) {
	if !s.iter.Next() {
		return nil, false
	}
	return s.iter.Row(), true
}


type Filter struct {
	child Operator
	pred func(Row) bool
}

func (f *Filter) Next() (Row, bool) {
	for {
		row, ok := f.child.Next()
		if !ok {
			return nil, false
		}
		if f.pred(row) {
			return row, true
		}
	}

}


type AggFunc func([]Row) interface{}

type GroupBy struct {
	child Operator
	groupField string
	aggs map[string]AggFunc
	data []Row
	idx int
}

func (g *GroupBy) buildGroups() {

	groups := make(map[interface{}][]Row)
	for  {
		row, ok := g.child.Next()
		if !ok {
			break
		}
		key := row[g.groupField]
		groups[key] = append(groups[key], row)
	}

	for key, rows := range groups {
		out := make(Row)
		out[g.groupField] = key
		for alias, fn := range g.aggs {
			out[alias] = fn(rows)
		}
		g.data = append(g.data, out)
	}

}

func (g *GroupBy) Next() (Row, bool) {
	if g.data == nil {
		g.buildGroups()
	}
	if g.idx >= len(g.data) {
		return nil, false
	}
	row := g.data[g.idx]
	g.idx++
	return row, true
}


type Project struct {
	child Operator
	fields []string
}

func (p *Project) Next() (Row, bool) {
	row, ok := p.child.Next()
	if !ok {
		return nil, false
	}
	out := make(Row)
	for _, field := range p.fields {
        out[field] = row[field]
    }
    return out, true
}


func Avg(field string) AggFunc {
	return func(rows []Row) interface{} {
		var sum float64
		var count int
		for _, row := range rows {
			if val, ok := row[field].(float64); ok {
				sum += val
				count++
			}
		}
		if count == 0 {
			return nil
		}
		return sum / float64(count)
	}
}
