<?php

/**
 * @param resource $refspec
 */
function git_refspec_direction($refspec) : int {}

/**
 * @param resource $refspec
 */
function git_refspec_dst($refspec) : string {}

/**
 * @param resource $refspec
 */
function git_refspec_dst_matches($refspec,string $refname) : bool {}

/**
 * @param resource $refspec
 */
function git_refspec_force($refspec) : bool {}

/**
 * @param resource $refspec
 */
function git_refspec_rtransform($refspec,string $name) : string {}

/**
 * @param resource $refspec
 */
function git_refspec_src($refspec) : string {}

/**
 * @param resource $refspec
 */
function git_refspec_src_matches($refspec,string $refname) : bool {}

/**
 * @param resource $refspec
 */
function git_refspec_string($refspec) : string {}

/**
 * @param resource $refspec
 */
function git_refspec_transform($refspec,string $name) : string {}
