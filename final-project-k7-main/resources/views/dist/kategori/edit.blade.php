@extends('dist.master')
@section('title')
Edit Category
@endsection
@section('content')
<form action="/kategori/{{$kategori->id}}" method="POST">
  @csrf
  @method('put')
  
  <div class="form-group">
    <label>Category</label>
    <input type="text" name="name" class="form-control" placeholder="Enter Category Name">
  </div>

  <button type="submit" class="btn btn-primary">Submit</button>
</form>

@endsection