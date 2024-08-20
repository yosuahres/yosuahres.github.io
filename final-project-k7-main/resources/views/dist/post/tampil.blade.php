@extends('dist.master')
@section('title')
@endsection
@section('content')
    <div class="row">
        <div class="col-6">
            <h2>Question Posted</h2>
        </div>
        @auth
            <div class="col-6 text-right">
                <a href="/post/create" class="btn btn-primary btn-sm">Add Question</a>
            </div>
        @endauth
    </div>

    <div class="row">
        @forelse ($post as $item)
            <div class="col-4">
                <div class="card">
                    <img src="{{ asset('image/' . $item->image) }}" class="card-img-top" alt="...">
                    <div class="card-body">
                        <p class="card-text">{{ $item->content }} {{ Str::limit($item->content, 10) }} </p>
                        <a href="/post/{{ $item->id }}" class="btn btn-primary btn-block">Read More</a>

                        <!-- Add a "Comment" button/link -->

                        @auth
                            <a href="/post/{{ $item->id }}/comment" class="btn btn-success btn-block">Comment</a>
                            <div class="row my-2">
                                <div class="col">
                                    <a href="/post/{{ $item->id }}/edit" class="btn btn-primary warning btn-block">Edit</a>
                                </div>
                                <div class="col">
                                    <form action="/post/{{ $item->id }}" method="POST">
                                        @csrf
                                        @method('delete')
                                        <input type="submit" value="Delete" class="btn btn-danger btn-block">
                                    </form>
                                </div>
                            </div>
                        @endauth

                    </div>
                </div>
            </div>
        @empty
            <div class="col-12">
                <h1>Nothing Posted</h1>
            </div>
        @endforelse
    </div>
@endsection
