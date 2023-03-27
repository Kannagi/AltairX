; ModuleID = 'test.c'
source_filename = "test.c"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-windows-msvc19.34.31935"

%struct.B = type { i32, i64, [0 x %struct.A] }
%struct.A = type { i8, [10 x [20 x i32]], i8 }
%struct.data = type { i8, i32, i32, i64 }

; Function Attrs: mustprogress nofree norecurse nosync nounwind readnone willreturn uwtable
define dso_local i64 @testmove(i64 noundef %0) local_unnamed_addr #0 {
  %2 = add i64 %0, 184467447091478
  ret i64 %2
}

; Function Attrs: nofree norecurse nosync nounwind readnone uwtable
define dso_local i32 @bar(i32 noundef %0, i32 noundef %1, i32 %2, i32 noundef %3) local_unnamed_addr #1 {
  %5 = icmp slt i32 %0, %3
  br i1 %5, label %6, label %12

6:                                                ; preds = %4, %6
  %7 = phi i32 [ %10, %6 ], [ %0, %4 ]
  %8 = phi i32 [ %9, %6 ], [ %1, %4 ]
  %9 = add nsw i32 %7, %8
  %10 = shl nsw i32 %9, 2
  %11 = icmp slt i32 %10, %3
  br i1 %11, label %6, label %12, !llvm.loop !4

12:                                               ; preds = %6, %4
  %13 = phi i32 [ %1, %4 ], [ %9, %6 ]
  %14 = phi i32 [ %0, %4 ], [ %10, %6 ]
  %15 = add nsw i32 %14, %13
  ret i32 %15
}

; Function Attrs: argmemonly nofree norecurse nosync nounwind uwtable
define dso_local void @foo(ptr nocapture noundef %0, i64 noundef %1) local_unnamed_addr #2 {
  store i32 1, ptr %0, align 4, !tbaa !7
  %3 = add i64 %1, -1
  %4 = icmp ugt i64 %3, 1
  br i1 %4, label %11, label %5

5:                                                ; preds = %2
  %6 = getelementptr inbounds i32, ptr %0, i64 %3
  store i32 2, ptr %6, align 4, !tbaa !7
  br label %10

7:                                                ; preds = %11
  %8 = getelementptr inbounds i32, ptr %0, i64 %3
  store i32 2, ptr %8, align 4, !tbaa !7
  %9 = icmp eq i64 %1, 0
  br i1 %9, label %18, label %10

10:                                               ; preds = %5, %7
  br label %19

11:                                               ; preds = %2, %11
  %12 = phi i64 [ %16, %11 ], [ 1, %2 ]
  %13 = getelementptr inbounds i32, ptr %0, i64 %12
  %14 = load i32, ptr %13, align 4, !tbaa !7
  %15 = shl nsw i32 %14, 1
  store i32 %15, ptr %13, align 4, !tbaa !7
  %16 = add nuw i64 %12, 1
  %17 = icmp eq i64 %16, %3
  br i1 %17, label %7, label %11, !llvm.loop !11

18:                                               ; preds = %19, %7
  ret void

19:                                               ; preds = %10, %19
  %20 = phi i64 [ %24, %19 ], [ 0, %10 ]
  %21 = getelementptr inbounds i32, ptr %0, i64 %20
  %22 = load i32, ptr %21, align 4, !tbaa !7
  %23 = add nsw i32 %22, 1
  store i32 %23, ptr %21, align 4, !tbaa !7
  %24 = add nuw i64 %20, 1
  %25 = icmp eq i64 %24, %1
  br i1 %25, label %18, label %19, !llvm.loop !12
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
define dso_local void @bubble_sort(ptr nocapture noundef %0, i64 noundef %1) local_unnamed_addr #2 {
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
  %11 = load i32, ptr %0, align 4, !tbaa !7
  br label %16

12:                                               ; preds = %13, %2
  ret void

13:                                               ; preds = %25, %5
  %14 = add nuw i64 %6, 1
  %15 = icmp eq i64 %14, %3
  br i1 %15, label %12, label %5, !llvm.loop !13

16:                                               ; preds = %10, %25
  %17 = phi i32 [ %26, %25 ], [ %11, %10 ]
  %18 = phi i64 [ %19, %25 ], [ 0, %10 ]
  %19 = add nuw i64 %18, 1
  %20 = getelementptr inbounds i32, ptr %0, i64 %19
  %21 = load i32, ptr %20, align 4, !tbaa !7
  %22 = icmp sgt i32 %17, %21
  br i1 %22, label %23, label %25

23:                                               ; preds = %16
  %24 = getelementptr inbounds i32, ptr %0, i64 %18
  store i32 %21, ptr %24, align 4, !tbaa !7
  store i32 %17, ptr %20, align 4, !tbaa !7
  br label %25

25:                                               ; preds = %16, %23
  %26 = phi i32 [ %21, %16 ], [ %17, %23 ]
  %27 = icmp ult i64 %19, %8
  br i1 %27, label %16, label %13, !llvm.loop !14
}

; Function Attrs: argmemonly mustprogress nofree norecurse nosync nounwind willreturn writeonly uwtable
define dso_local void @make_data(ptr noalias nocapture writeonly sret(%struct.data) align 8 %0, i8 noundef %1, i32 noundef %2, i32 noundef %3, i64 noundef %4) local_unnamed_addr #3 {
  store i8 %1, ptr %0, align 8, !tbaa !15
  %6 = getelementptr inbounds %struct.data, ptr %0, i64 0, i32 1
  store i32 %2, ptr %6, align 4, !tbaa !18
  %7 = getelementptr inbounds %struct.data, ptr %0, i64 0, i32 2
  store i32 %3, ptr %7, align 8, !tbaa !19
  %8 = getelementptr inbounds %struct.data, ptr %0, i64 0, i32 3
  store i64 %4, ptr %8, align 8, !tbaa !20
  %9 = zext i8 %1 to i32
  %10 = icmp ugt i32 %9, %2
  br i1 %10, label %11, label %13

11:                                               ; preds = %5
  %12 = trunc i32 %2 to i8
  store i8 %12, ptr %0, align 8, !tbaa !15
  br label %20

13:                                               ; preds = %5
  %14 = zext i32 %3 to i64
  %15 = icmp ugt i64 %14, %4
  br i1 %15, label %16, label %20

16:                                               ; preds = %13
  %17 = zext i8 %1 to i64
  %18 = add i64 %17, %4
  %19 = trunc i64 %18 to i32
  store i32 %19, ptr %7, align 8, !tbaa !19
  br label %20

20:                                               ; preds = %13, %16, %11
  ret void
}

; Function Attrs: argmemonly mustprogress nofree norecurse nosync nounwind readonly willreturn uwtable
define dso_local i64 @use_data(ptr nocapture noundef readonly %0) local_unnamed_addr #4 {
  %2 = load i8, ptr %0, align 8, !tbaa !15
  %3 = zext i8 %2 to i64
  %4 = getelementptr inbounds %struct.data, ptr %0, i64 0, i32 3
  %5 = load i64, ptr %4, align 8, !tbaa !20
  %6 = mul i64 %5, %3
  %7 = getelementptr inbounds %struct.data, ptr %0, i64 0, i32 2
  %8 = load i32, ptr %7, align 8, !tbaa !19
  %9 = zext i32 %8 to i64
  %10 = add i64 %6, %9
  ret i64 %10
}

; Function Attrs: nounwind uwtable
define dso_local ptr @external_func_user(ptr noundef %0, ptr noundef readnone %1) local_unnamed_addr #5 {
  %3 = icmp eq ptr %0, %1
  br i1 %3, label %14, label %4

4:                                                ; preds = %2, %4
  %5 = phi ptr [ %12, %4 ], [ %0, %2 ]
  %6 = load i32, ptr %5, align 4, !tbaa !7
  %7 = call i32 @external_func(i32 noundef %6) #7
  %8 = load i32, ptr %5, align 4, !tbaa !7
  %9 = mul nsw i32 %8, %7
  store i32 %9, ptr %5, align 4, !tbaa !7
  %10 = shl nsw i32 %9, 1
  %11 = call i32 @external_func(i32 noundef %10) #7
  store i32 %11, ptr %5, align 4, !tbaa !7
  %12 = getelementptr inbounds i32, ptr %5, i64 1
  %13 = icmp eq ptr %12, %1
  br i1 %13, label %14, label %4, !llvm.loop !21

14:                                               ; preds = %4, %2
  %15 = phi ptr [ %0, %2 ], [ %1, %4 ]
  ret ptr %15
}

declare dso_local i32 @external_func(i32 noundef) local_unnamed_addr #6

; Function Attrs: nofree norecurse nosync nounwind readnone uwtable
define dso_local i32 @main() local_unnamed_addr #1 {
  ret i32 25
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind readnone willreturn uwtable "disable-tail-calls"="true" "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nofree norecurse nosync nounwind readnone uwtable "disable-tail-calls"="true" "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { argmemonly nofree norecurse nosync nounwind uwtable "disable-tail-calls"="true" "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { argmemonly mustprogress nofree norecurse nosync nounwind willreturn writeonly uwtable "disable-tail-calls"="true" "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { argmemonly mustprogress nofree norecurse nosync nounwind readonly willreturn uwtable "disable-tail-calls"="true" "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { nounwind uwtable "disable-tail-calls"="true" "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #6 = { "disable-tail-calls"="true" "frame-pointer"="none" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #7 = { nounwind }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 2}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{!"clang version 15.0.1"}
!4 = distinct !{!4, !5, !6}
!5 = !{!"llvm.loop.mustprogress"}
!6 = !{!"llvm.loop.unroll.disable"}
!7 = !{!8, !8, i64 0}
!8 = !{!"int", !9, i64 0}
!9 = !{!"omnipotent char", !10, i64 0}
!10 = !{!"Simple C/C++ TBAA"}
!11 = distinct !{!11, !5, !6}
!12 = distinct !{!12, !5, !6}
!13 = distinct !{!13, !5, !6}
!14 = distinct !{!14, !5, !6}
!15 = !{!16, !9, i64 0}
!16 = !{!"data", !9, i64 0, !8, i64 4, !8, i64 8, !17, i64 16}
!17 = !{!"long long", !9, i64 0}
!18 = !{!16, !8, i64 4}
!19 = !{!16, !8, i64 8}
!20 = !{!16, !17, i64 16}
!21 = distinct !{!21, !5, !6}
