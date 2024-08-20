@extends('dist.master')
@section('title')
Add Question
@endsection

@section('content')
<form action="/post/{{$post->id}}" method="POST" enctype="multipart/form-data">
  @csrf
  @method('put')
  <div class="form-group">
    <label>Question Category</label>
    <select class="form-control" name="kategori_id">
        <option value="">--Select Category--</option>
      @forelse ($kategori as $item)
        @if ($item->id === $post->kategori_id)
        <option value="{{$item->id}}" selected>{{$item->name}} </option>
        @else
        <option value="{{$item->id}}">{{$item->name}} </option>
        @endif
      @empty
          <option value="">No Category</option>
      @endforelse
    </select>
  </div>

  <div class="form-group">
    <label>Question Ask</label>
    <textarea class="form-control" name="content" cols="30" rows="10" placeholder="What's your question?">{{$post->content}}</textarea>
  </div>

  <div class="form-group">
    <label>Question Image</label>
    <input type="file" class="form-control" name="image">
  </div>

  <button type="submit" class="btn btn-primary">Add question</button>
</form>
@endsection