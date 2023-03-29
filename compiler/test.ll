; ModuleID = 'test.c'
source_filename = "test.c"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-windows-msvc19.34.31935"

%struct.B = type { i32, i64, [0 x %struct.A] }
%struct.A = type { i8, [10 x [20 x i32]], i8 }
%struct.data = type { i8, i32, i32, i64 }

@global_int = dso_local local_unnamed_addr global i32 0, align 4
@global_str = internal constant [13 x i8] c"Hello world!\00", align 1
@base64_table = internal unnamed_addr constant [128 x i8] c"\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00>\00\00\00?456789:;<=\00\00\00\00\00\00\00\00\01\02\03\04\05\06\07\08\09\0A\0B\0C\0D\0E\0F\10\11\12\13\14\15\16\17\18\19\00\00\00\00\00\00\1A\1B\1C\1D\1E\1F !\22#$%&'()*+,-./0123\00\00\00\00\00", align 16

; Function Attrs: mustprogress nofree norecurse nosync nounwind readnone willreturn uwtable
define dso_local nonnull ptr @get_message() local_unnamed_addr #0 {
  ret ptr @global_str
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn writeonly uwtable
define dso_local void @set_global(i32 noundef %0) local_unnamed_addr #1 {
  store i32 %0, ptr @global_int, align 4, !tbaa !4
  ret void
}

; Function Attrs: nofree norecurse nosync nounwind uwtable
define dso_local void @parse_base64(ptr nocapture noundef readonly %0, i64 noundef %1, ptr nocapture noundef writeonly %2) local_unnamed_addr #2 {
  %4 = icmp eq i64 %1, 0
  br i1 %4, label %5, label %6

5:                                                ; preds = %6, %3
  ret void

6:                                                ; preds = %3, %6
  %7 = phi ptr [ %52, %6 ], [ %2, %3 ]
  %8 = phi i64 [ %53, %6 ], [ 0, %3 ]
  %9 = getelementptr inbounds i8, ptr %0, i64 %8
  %10 = load i8, ptr %9, align 1, !tbaa !8
  %11 = and i8 %10, 127
  %12 = zext i8 %11 to i64
  %13 = getelementptr inbounds [128 x i8], ptr @base64_table, i64 0, i64 %12
  %14 = load i8, ptr %13, align 1, !tbaa !8
  %15 = zext i8 %14 to i32
  %16 = shl nuw nsw i32 %15, 18
  %17 = or i64 %8, 1
  %18 = getelementptr inbounds i8, ptr %0, i64 %17
  %19 = load i8, ptr %18, align 1, !tbaa !8
  %20 = and i8 %19, 127
  %21 = zext i8 %20 to i64
  %22 = getelementptr inbounds [128 x i8], ptr @base64_table, i64 0, i64 %21
  %23 = load i8, ptr %22, align 1, !tbaa !8
  %24 = zext i8 %23 to i32
  %25 = shl nuw nsw i32 %24, 12
  %26 = or i32 %25, %16
  %27 = or i64 %8, 2
  %28 = getelementptr inbounds i8, ptr %0, i64 %27
  %29 = load i8, ptr %28, align 1, !tbaa !8
  %30 = and i8 %29, 127
  %31 = zext i8 %30 to i64
  %32 = getelementptr inbounds [128 x i8], ptr @base64_table, i64 0, i64 %31
  %33 = load i8, ptr %32, align 1, !tbaa !8
  %34 = zext i8 %33 to i32
  %35 = shl nuw nsw i32 %34, 6
  %36 = or i32 %35, %25
  %37 = or i64 %8, 3
  %38 = getelementptr inbounds i8, ptr %0, i64 %37
  %39 = load i8, ptr %38, align 1, !tbaa !8
  %40 = and i8 %39, 127
  %41 = zext i8 %40 to i64
  %42 = getelementptr inbounds [128 x i8], ptr @base64_table, i64 0, i64 %41
  %43 = load i8, ptr %42, align 1, !tbaa !8
  %44 = lshr i32 %26, 16
  %45 = trunc i32 %44 to i8
  %46 = getelementptr inbounds i8, ptr %7, i64 1
  store i8 %45, ptr %7, align 1, !tbaa !8
  %47 = lshr i32 %36, 8
  %48 = trunc i32 %47 to i8
  %49 = getelementptr inbounds i8, ptr %7, i64 2
  store i8 %48, ptr %46, align 1, !tbaa !8
  %50 = trunc i32 %35 to i8
  %51 = or i8 %43, %50
  %52 = getelementptr inbounds i8, ptr %7, i64 3
  store i8 %51, ptr %49, align 1, !tbaa !8
  %53 = add nuw i64 %8, 4
  %54 = icmp ult i64 %53, %1
  br i1 %54, label %6, label %5, !llvm.loop !9
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind readnone willreturn uwtable
define dso_local i64 @testmove(i64 noundef %0) local_unnamed_addr #0 {
  %2 = add i64 %0, 184467447091478
  ret i64 %2
}

; Function Attrs: nofree norecurse nosync nounwind readnone uwtable
define dso_local i32 @bar(i32 noundef %0, i32 noundef %1, i32 %2, i32 noundef %3) local_unnamed_addr #3 {
  %5 = icmp slt i32 %0, %3
  br i1 %5, label %6, label %12

6:                                                ; preds = %4, %6
  %7 = phi i32 [ %10, %6 ], [ %0, %4 ]
  %8 = phi i32 [ %9, %6 ], [ %1, %4 ]
  %9 = add nsw i32 %7, %8
  %10 = shl nsw i32 %9, 2
  %11 = icmp slt i32 %10, %3
  br i1 %11, label %6, label %12, !llvm.loop !12

12:                                               ; preds = %6, %4
  %13 = phi i32 [ %1, %4 ], [ %9, %6 ]
  %14 = phi i32 [ %0, %4 ], [ %10, %6 ]
  %15 = add nsw i32 %14, %13
  ret i32 %15
}

; Function Attrs: argmemonly nofree norecurse nosync nounwind uwtable
define dso_local void @foo(ptr nocapture noundef %0, i64 noundef %1) local_unnamed_addr #4 {
  store i32 1, ptr %0, align 4, !tbaa !4
  %3 = add i64 %1, -1
  %4 = icmp ugt i64 %3, 1
  br i1 %4, label %11, label %5

5:                                                ; preds = %2
  %6 = getelementptr inbounds i32, ptr %0, i64 %3
  store i32 2, ptr %6, align 4, !tbaa !4
  br label %10

7:                                                ; preds = %11
  %8 = getelementptr inbounds i32, ptr %0, i64 %3
  store i32 2, ptr %8, align 4, !tbaa !4
  %9 = icmp eq i64 %1, 0
  br i1 %9, label %18, label %10

10:                                               ; preds = %5, %7
  br label %19

11:                                               ; preds = %2, %11
  %12 = phi i64 [ %16, %11 ], [ 1, %2 ]
  %13 = getelementptr inbounds i32, ptr %0, i64 %12
  %14 = load i32, ptr %13, align 4, !tbaa !4
  %15 = shl nsw i32 %14, 1
  store i32 %15, ptr %13, align 4, !tbaa !4
  %16 = add nuw i64 %12, 1
  %17 = icmp eq i64 %16, %3
  br i1 %17, label %7, label %11, !llvm.loop !13

18:                                               ; preds = %19, %7
  ret void

19:                                               ; preds = %10, %19
  %20 = phi i64 [ %24, %19 ], [ 0, %10 ]
  %21 = getelementptr inbounds i32, ptr %0, i64 %20
  %22 = load i32, ptr %21, align 4, !tbaa !4
  %23 = add nsw i32 %22, 1
  store i32 %23, ptr %21, align 4, !tbaa !4
  %24 = add nuw i64 %20, 1
  %25 = icmp eq i64 %24, %1
  br i1 %25, label %18, label %19, !llvm.loop !14
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind readnone willreturn uwtable
define dso_local nonnull ptr @testptr(ptr noundef readnone %0, i64 noundef %1) local_unnamed_addr #0 {
  %3 = getelementptr inbounds %struct.B, ptr %0, i64 0, i32 2, i64 %1, i32 1, i64 5, i64 13
  ret ptr %3
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind readnone willreturn uwtable
define dso_local i32 @negate(i32 noundef %0) local_unnamed_addr #0 {
  %2 = sub nsw i32 0, %0
  ret i32 %2
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind readnone willreturn uwtable
define dso_local i32 @sext(i16 noundef %0) local_unnamed_addr #0 {
  %2 = sext i16 %0 to i32
  ret i32 %2
}

; Function Attrs: argmemonly nofree norecurse nosync nounwind uwtable
define dso_local void @bubble_sort(ptr nocapture noundef %0, i64 noundef %1) local_unnamed_addr #4 {
  %3 = add i64 %1, -1
  %4 = icmp eq i64 %3, 0
  br i1 %4, label %12, label %5

5:                                                ; preds = %2, %13
  %6 = phi i64 [ %14, %13 ], [ 0, %2 ]
  %7 = xor i64 %6, -1
  %8 = add i64 %7, %1
  %9 = icmp eq i64 %8, 0
  br i1 %9, label %13, label %10

10:                                               ; preds = %5
  %11 = load i32, ptr %0, align 4, !tbaa !4
  br label %16

12:                                               ; preds = %13, %2
  ret void

13:                                               ; preds = %25, %5
  %14 = add nuw i64 %6, 1
  %15 = icmp eq i64 %14, %3
  br i1 %15, label %12, label %5, !llvm.loop !15

16:                                               ; preds = %10, %25
  %17 = phi i32 [ %26, %25 ], [ %11, %10 ]
  %18 = phi i64 [ %19, %25 ], [ 0, %10 ]
  %19 = add nuw i64 %18, 1
  %20 = getelementptr inbounds i32, ptr %0, i64 %19
  %21 = load i32, ptr %20, align 4, !tbaa !4
  %22 = icmp sgt i32 %17, %21
  br i1 %22, label %23, label %25

23:                                               ; preds = %16
  %24 = getelementptr inbounds i32, ptr %0, i64 %18
  store i32 %21, ptr %24, align 4, !tbaa !4
  store i32 %17, ptr %20, align 4, !tbaa !4
  br label %25

25:                                               ; preds = %16, %23
  %26 = phi i32 [ %21, %16 ], [ %17, %23 ]
  %27 = icmp ult i64 %19, %8
  br i1 %27, label %16, label %13, !llvm.loop !16
}

; Function Attrs: argmemonly mustprogress nofree norecurse nosync nounwind willreturn writeonly uwtable
define dso_local void @make_data(ptr noalias nocapture writeonly sret(%struct.data) align 8 %0, i8 noundef %1, i32 noundef %2, i32 noundef %3, i64 noundef %4) local_unnamed_addr #5 {
  store i8 %1, ptr %0, align 8, !tbaa !17
  %6 = getelementptr inbounds %struct.data, ptr %0, i64 0, i32 1
  store i32 %2, ptr %6, align 4, !tbaa !20
  %7 = getelementptr inbounds %struct.data, ptr %0, i64 0, i32 2
  store i32 %3, ptr %7, align 8, !tbaa !21
  %8 = getelementptr inbounds %struct.data, ptr %0, i64 0, i32 3
  store i64 %4, ptr %8, align 8, !tbaa !22
  %9 = zext i8 %1 to i32
  %10 = icmp ugt i32 %9, %2
  br i1 %10, label %11, label %13

11:                                               ; preds = %5
  %12 = trunc i32 %2 to i8
  store i8 %12, ptr %0, align 8, !tbaa !17
  br label %20

13:                                               ; preds = %5
  %14 = zext i32 %3 to i64
  %15 = icmp ugt i64 %14, %4
  br i1 %15, label %16, label %20

16:                                               ; preds = %13
  %17 = zext i8 %1 to i64
  %18 = add i64 %17, %4
  %19 = trunc i64 %18 to i32
  store i32 %19, ptr %7, align 8, !tbaa !21
  br label %20

20:                                               ; preds = %13, %16, %11
  ret void
}

; Function Attrs: argmemonly mustprogress nofree norecurse nosync nounwind readonly willreturn uwtable
define dso_local i64 @use_data(ptr nocapture noundef readonly %0) local_unnamed_addr #6 {
  %2 = load i8, ptr %0, align 8, !tbaa !17
  %3 = zext i8 %2 to i64
  %4 = getelementptr inbounds %struct.data, ptr %0, i64 0, i32 3
  %5 = load i64, ptr %4, align 8, !tbaa !22
  %6 = mul i64 %5, %3
  %7 = getelementptr inbounds %struct.data, ptr %0, i64 0, i32 2
  %8 = load i32, ptr %7, align 8, !tbaa !21
  %9 = zext i32 %8 to i64
  %10 = add i64 %6, %9
  ret i64 %10
}

; Function Attrs: nounwind uwtable
define dso_local ptr @external_func_user(ptr noundef %0, ptr noundef readnone %1) local_unnamed_addr #7 {
  %3 = icmp eq ptr %0, %1
  br i1 %3, label %14, label %4

4:                                                ; preds = %2, %4
  %5 = phi ptr [ %12, %4 ], [ %0, %2 ]
  %6 = load i32, ptr %5, align 4, !tbaa !4
  %7 = call i32 @external_func(i32 noundef %6) #9
  %8 = load i32, ptr %5, align 4, !tbaa !4
  %9 = mul nsw i32 %8, %7
  store i32 %9, ptr %5, align 4, !tbaa !4
  %10 = shl nsw i32 %9, 1
  %11 = call i32 @external_func(i32 noundef %10) #9
  store i32 %11, ptr %5, align 4, !tbaa !4
  %12 = getelementptr inbounds i32, ptr %5, i64 1
  %13 = icmp eq ptr %12, %1
  br i1 %13, label %14, label %4, !llvm.loop !23

14:                                               ; preds = %4, %2
  %15 = phi ptr [ %0, %2 ], [ %1, %4 ]
  ret ptr %15
}

declare dso_local i32 @external_func(i32 noundef) local_unnamed_addr #8

; Function Attrs: nofree norecurse nosync nounwind readnone uwtable
define dso_local i32 @main() local_unnamed_addr #3 {
  ret i32 25
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind readnone willreturn uwtable "disable-tail-calls"="true" "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { mustprogress nofree norecurse nosync nounwind willreturn writeonly uwtable "disable-tail-calls"="true" "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nofree norecurse nosync nounwind uwtable "disable-tail-calls"="true" "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nofree norecurse nosync nounwind readnone uwtable "disable-tail-calls"="true" "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { argmemonly nofree norecurse nosync nounwind uwtable "disable-tail-calls"="true" "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { argmemonly mustprogress nofree norecurse nosync nounwind willreturn writeonly uwtable "disable-tail-calls"="true" "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #6 = { argmemonly mustprogress nofree norecurse nosync nounwind readonly willreturn uwtable "disable-tail-calls"="true" "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #7 = { nounwind uwtable "disable-tail-calls"="true" "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #8 = { "disable-tail-calls"="true" "frame-pointer"="none" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #9 = { nounwind }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 2}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{!"clang version 15.0.1"}
!4 = !{!5, !5, i64 0}
!5 = !{!"int", !6, i64 0}
!6 = !{!"omnipotent char", !7, i64 0}
!7 = !{!"Simple C/C++ TBAA"}
!8 = !{!6, !6, i64 0}
!9 = distinct !{!9, !10, !11}
!10 = !{!"llvm.loop.mustprogress"}
!11 = !{!"llvm.loop.unroll.disable"}
!12 = distinct !{!12, !10, !11}
!13 = distinct !{!13, !10, !11}
!14 = distinct !{!14, !10, !11}
!15 = distinct !{!15, !10, !11}
!16 = distinct !{!16, !10, !11}
!17 = !{!18, !6, i64 0}
!18 = !{!"data", !6, i64 0, !5, i64 4, !5, i64 8, !19, i64 16}
!19 = !{!"long long", !6, i64 0}
!20 = !{!18, !5, i64 4}
!21 = !{!18, !5, i64 8}
!22 = !{!18, !19, i64 16}
!23 = distinct !{!23, !10, !11}
