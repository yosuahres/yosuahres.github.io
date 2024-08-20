<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use App\Models\Category;
use App\Models\Post;
use Illuminate\Support\Facades\DB;
use Illuminate\Support\Facades\File;

class PostController extends Controller
{
    public function __construct()
    {
        $this->middleware('auth')->except('index', 'show');
    }
    /**
     * Display a listing of the resource.
     *
     * @return \Illuminate\Http\Response
     */
    public function index()
    {
        $post = Post::all();
        return view('dist.post.tampil', ['post' => $post]);
    }

    /**
     * Show the form for creating a new resource.
     *
     * @return \Illuminate\Http\Response
     */
    public function create()
    {
        $kategori = category::all();
        return view('dist.post.tambah',['kategori' => $kategori]);
    }

    /**
     * Store a newly created resource in storage.
     *
     * @param  \Illuminate\Http\Request  $request
     * @return \Illuminate\Http\Response
     */
    public function store(Request $request)
    {

        // dd($request->all());
        $request->validate([
            'content' => 'required|min:5|max:1500',
            'image' => 'required|mimes:png,jpeg,jpg|max:2048',
            'kategori_id' => 'required',

        ]);
        //mengganti nama file
        $imageName = time().'.'.$request->image->extension();
        //tempat simpen file
        $request->image->move(public_path('image'), $imageName);

        $kategori = new Post;
        $kategori->content = $request->input('content');
        $kategori->kategori_id = $request->input('kategori_id');
        $kategori->image = $imageName;
        $kategori->save();

        return redirect('/post');
    }

    /**
     * Display the specified resource.
     *
     * @param  int  $id
     * @return \Illuminate\Http\Response
     */
    public function show($id)
    {
        $post = Post::find($id);

        return view('dist.post.detail', ['post' => $post]);
    }

    /**
     * Show the form for editing the specified resource.
     *
     * @param  int  $id
     * @return \Illuminate\Http\Response
     */
    public function edit($id)
    {
        $post = Post::find($id);
        $kategori = Category::all();

        return view('dist.post.edit', ['post' => $post,'kategori' => $kategori]);
    }

    /**
     * Update the specified resource in storage.
     *
     * @param  \Illuminate\Http\Request  $request
     * @param  int  $id
     * @return \Illuminate\Http\Response
     */
    public function update(Request $request, $id)
    {
        $request->validate([
            'content' => 'required|min:5|max:1500',
            'image' => 'mimes:png,jpeg,jpg|max:2048',
            'kategori_id' => 'required',
        ]);

        $post = Post::find($id);

        if($request->has('image')){
            $path= "image/";
            File::delete($path . $post->image);
            $image_name = time().'.'.$request->image->extension();
            $request->image->move(public_path('image'). $image_name);
            $post->image = $image_name;
        }

        $post->content = $request->input('content');
        $post->kategori_id = $request->input('kategori_id');

        $post->save();
    }

    /**
     * Remove the specified resource from storage.
     *
     * @param  int  $id
     * @return \Illuminate\Http\Response
     */
    public function destroy($id)
    {
        $post = Post::find($id);

        $path ="image/";
        File::delete($path . $post->image);

        $post -> delete();

        return redirect('/post');

    }
}
