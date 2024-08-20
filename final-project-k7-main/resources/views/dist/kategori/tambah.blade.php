@extends('dist.master')
@section('title')
Add Category
@endsection

@section('content')
<form action="/kategori" method="POST">
  @csrf
  <div class="form-group">
    <label>Category</label>
    <input type="text" name="name" class="form-control" placeholder="Enter Category Name">
  </div>

  <button type="submit" class="btn btn-primary">Add Category</button>
</form>
@endsection