@extends('dist.master')
@section('title')
Add Question
@endsection

@section('content')
<form action="/post" method="POST" enctype="multipart/form-data">
  @csrf
  
  <div class="form-group">
    <label>Question Category</label>
    <select class="form-control" name="kategori_id">
        <option value="">--Select Category--</option>
      @forelse ($kategori as $item)
          <option value="{{$item->id}}">{{$item->name}} </option>
      @empty
          <option value="">No Category</option>
      @endforelse
    </select>
  </div>

  <div class="form-group">
    <label>Question Ask</label>
    <textarea class="form-control" name="content" cols="30" rows="10" placeholder="What's your question?"></textarea>
  </div>

  <div class="form-group">
    <label>Question Image</label>
    <input type="file" class="form-control" name="image">
  </div>

  <button type="submit" class="btn btn-primary">Add question</button>
</form>
@endsection