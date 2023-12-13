<template>
  <div>
    <PostView v-for="post in posts" :key="post.id" :post="post" :full="false" />
  </div>
</template>

<script>
import PostView from "./PostView";

export default {
  name: "IndexView",
  components: {
    PostView
  },
  props: ["posts"]
};
</script>

<style scoped></style>
