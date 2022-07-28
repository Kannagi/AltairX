; ModuleID = 'test.c'
source_filename = "test.c"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-windows-msvc19.29.30136"

%struct.data = type { i8, i32, i32, i64 }

; Function Attrs: norecurse nounwind readnone uwtable
define dso_local i32 @bar(i32 %0, i32 %1, i32 %2, i32 %3) local_unnamed_addr #0 {
  %5 = icmp slt i32 %0, %3
  br i1 %5, label %6, label %12

6:                                                ; preds = %4, %6
  %7 = phi i32 [ %10, %6 ], [ %0, %4 ]
  %8 = phi i32 [ %9, %6 ], [ %1, %4 ]
  %9 = add nsw i32 %7, %8
  %10 = shl nsw i32 %9, 2
  %11 = icmp slt i32 %10, %3
  br i1 %11, label %6, label %12, !llvm.loop !3

12:                                               ; preds = %6, %4
  %13 = phi i32 [ %1, %4 ], [ %9, %6 ]
  %14 = phi i32 [ %0, %4 ], [ %10, %6 ]
  %15 = add nsw i32 %14, %13
  ret i32 %15
}

; Function Attrs: nofree norecurse nounwind uwtable
define dso_local void @foo(i32* nocapture %0, i64 %1) local_unnamed_addr #1 {
  store i32 1, i32* %0, align 4, !tbaa !5
  %3 = add i64 %1, -1
  %4 = icmp ugt i64 %3, 1
  br i1 %4, label %11, label %5

5:                                                ; preds = %2
  %6 = getelementptr inbounds i32, i32* %0, i64 %3
  store i32 2, i32* %6, align 4, !tbaa !5
  br label %10

7:                                                ; preds = %11
  %8 = getelementptr inbounds i32, i32* %0, i64 %3
  store i32 2, i32* %8, align 4, !tbaa !5
  %9 = icmp eq i64 %1, 0
  br i1 %9, label %18, label %10

10:                                               ; preds = %5, %7
  br label %19

11:                                               ; preds = %2, %11
  %12 = phi i64 [ %16, %11 ], [ 1, %2 ]
  %13 = getelementptr inbounds i32, i32* %0, i64 %12
  %14 = load i32, i32* %13, align 4, !tbaa !5
  %15 = shl nsw i32 %14, 1
  store i32 %15, i32* %13, align 4, !tbaa !5
  %16 = add nuw i64 %12, 1
  %17 = icmp eq i64 %16, %3
  br i1 %17, label %7, label %11, !llvm.loop !9

18:                                               ; preds = %19, %7
  ret void

19:                                               ; preds = %10, %19
  %20 = phi i64 [ %24, %19 ], [ 0, %10 ]
  %21 = getelementptr inbounds i32, i32* %0, i64 %20
  %22 = load i32, i32* %21, align 4, !tbaa !5
  %23 = add nsw i32 %22, 1
  store i32 %23, i32* %21, align 4, !tbaa !5
  %24 = add nuw i64 %20, 1
  %25 = icmp eq i64 %24, %1
  br i1 %25, label %18, label %19, !llvm.loop !10
}

; Function Attrs: nofree norecurse nounwind uwtable
define dso_local void @bubble_sort(i32* nocapture %0, i64 %1) local_unnamed_addr #1 {
  %3 = add i64 %1, -1
  %4 = icmp eq i64 %3, 0
  br i1 %4, label %14, label %5

5:                                                ; preds = %2
  %6 = sub i64 0, %1
  br label %7

7:                                                ; preds = %15, %5
  %8 = phi i64 [ %3, %5 ], [ %17, %15 ]
  %9 = phi i64 [ 0, %5 ], [ %16, %15 ]
  %10 = xor i64 %9, -1
  %11 = icmp eq i64 %10, %6
  br i1 %11, label %15, label %12

12:                                               ; preds = %7
  %13 = load i32, i32* %0, align 4, !tbaa !5
  br label %19

14:                                               ; preds = %15, %2
  ret void

15:                                               ; preds = %28, %7
  %16 = add nuw i64 %9, 1
  %17 = add i64 %8, -1
  %18 = icmp eq i64 %16, %3
  br i1 %18, label %14, label %7, !llvm.loop !11

19:                                               ; preds = %28, %12
  %20 = phi i32 [ %29, %28 ], [ %13, %12 ]
  %21 = phi i64 [ %22, %28 ], [ 0, %12 ]
  %22 = add nuw i64 %21, 1
  %23 = getelementptr inbounds i32, i32* %0, i64 %22
  %24 = load i32, i32* %23, align 4, !tbaa !5
  %25 = icmp sgt i32 %20, %24
  br i1 %25, label %26, label %28

26:                                               ; preds = %19
  %27 = getelementptr inbounds i32, i32* %0, i64 %21
  store i32 %24, i32* %27, align 4, !tbaa !5
  store i32 %20, i32* %23, align 4, !tbaa !5
  br label %28

28:                                               ; preds = %19, %26
  %29 = phi i32 [ %24, %19 ], [ %20, %26 ]
  %30 = icmp eq i64 %22, %8
  br i1 %30, label %15, label %19, !llvm.loop !12
}

; Function Attrs: nofree norecurse nounwind uwtable writeonly
define dso_local void @make_data(%struct.data* noalias nocapture sret align 8 %0, i8 %1, i32 %2, i32 %3, i64 %4) local_unnamed_addr #2 {
  %6 = getelementptr inbounds %struct.data, %struct.data* %0, i64 0, i32 0
  store i8 %1, i8* %6, align 8, !tbaa !13
  %7 = getelementptr inbounds %struct.data, %struct.data* %0, i64 0, i32 1
  store i32 %2, i32* %7, align 4, !tbaa !16
  %8 = getelementptr inbounds %struct.data, %struct.data* %0, i64 0, i32 2
  store i32 %3, i32* %8, align 8, !tbaa !17
  %9 = getelementptr inbounds %struct.data, %struct.data* %0, i64 0, i32 3
  store i64 %4, i64* %9, align 8, !tbaa !18
  %10 = zext i8 %1 to i32
  %11 = icmp ugt i32 %10, %2
  br i1 %11, label %12, label %14

12:                                               ; preds = %5
  %13 = trunc i32 %2 to i8
  store i8 %13, i8* %6, align 8, !tbaa !13
  br label %21

14:                                               ; preds = %5
  %15 = zext i32 %3 to i64
  %16 = icmp ugt i64 %15, %4
  br i1 %16, label %17, label %21

17:                                               ; preds = %14
  %18 = zext i8 %1 to i64
  %19 = add i64 %18, %4
  %20 = trunc i64 %19 to i32
  store i32 %20, i32* %8, align 8, !tbaa !17
  br label %21

21:                                               ; preds = %14, %17, %12
  ret void
}

; Function Attrs: norecurse nounwind readnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  ret i32 25
}

attributes #0 = { norecurse nounwind readnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nofree norecurse nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nofree norecurse nounwind uwtable writeonly "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 2}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{!"clang version 11.0.0"}
!3 = distinct !{!3, !4}
!4 = !{!"llvm.loop.unroll.disable"}
!5 = !{!6, !6, i64 0}
!6 = !{!"int", !7, i64 0}
!7 = !{!"omnipotent char", !8, i64 0}
!8 = !{!"Simple C/C++ TBAA"}
!9 = distinct !{!9, !4}
!10 = distinct !{!10, !4}
!11 = distinct !{!11, !4}
!12 = distinct !{!12, !4}
!13 = !{!14, !7, i64 0}
!14 = !{!"data", !7, i64 0, !6, i64 4, !6, i64 8, !15, i64 16}
!15 = !{!"long long", !7, i64 0}
!16 = !{!14, !6, i64 4}
!17 = !{!14, !6, i64 8}
!18 = !{!14, !15, i64 16}
