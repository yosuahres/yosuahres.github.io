<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use Illuminate\Support\Facades\DB;

class KategoriController extends Controller
{
    public function create()
    {
        return view('dist.kategori.tambah');
    }

    public function store(Request $request)
    {
        // dd($request->all());

        // Validation
        $request->validate([
            'name' => 'required|min:5',
        ]);

        DB::table('kategori')->insert([
            'name' => $request->input('name'),  
        ]);
         return redirect('/kategori');
    }

    //user post + user post lain
    public function index(){
        $kategori = DB::table('kategori')->get();
        return view('dist.kategori.tampil', ['kategori'=>$kategori]);
    }

    //semua yang user post sendiri
    public function show($id){
        $kategori = DB::table('kategori')->find($id);
        return view('dist.kategori.detail', ['kategori' => $kategori]);
    }

    //edit post user dari master??
    public function edit($id){
        $kategori = DB::table('kategori')->find($id);
        return view('dist.kategori.edit', ['kategori' => $kategori]);
    }

    //di edit, update data
    public function update(Request $request, $id){
    $request->validate([
        'name' => 'required|min:5',
    ]);

    DB::table('kategori')
        ->where('id',$id)
        ->update([
            'name' => $request->input('name'),  
        ]);

        return redirect('/kategori');
    }

    //ngedelete post user data;
    public function destroy($id){
        DB::table('kategori')->where('id',$id)->delete();
        return redirect('/kategori');
    }
}
