@extends('dist.master')
@section('title')
    Category List
@endsection

@section('content')
    <a href="/kategori/create" class="btn btn primary btn-sm my-3 bg-primary text-white">Add Category</a>

    <table class="table">
        <thead>
            <tr>
                <th scope="col">#</th>
                <th scope="col">Category</th>
                <th scope="col">Action</th>
            </tr>
        </thead>
        <tbody>
            @forelse ($kategori as $key=>$item)
                <tr>
                    <th scope="row">{{ $key + 1 }}</th>
                    <td>{{ $item->name }}</td>
                    <td>
                        <form action="/kategori/{{ $item->id }}" method="POST">
                            @csrf
                            @method('delete')
                            <a href="/kategori/{{ $item->id }}" class="btn btn-sm btn-info">Detail</a>
                            <a href="/kategori/{{ $item->id }}/edit" class="btn btn-sm btn-warning">Edit</a>
                            <input type="submit" value="Delete" class="btn btn-sm btn-danger">
                        </form>
                    </td>
                </tr>
            @empty
                <tr>
                    <td>No Category Posted</td>
                </tr>
            @endforelse
        </tbody>
    </table>
@endsection
