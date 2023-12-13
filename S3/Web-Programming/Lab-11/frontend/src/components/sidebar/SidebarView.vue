<template>
  <aside>
    <SidebarPost v-for="post in posts" :key="post.id" :post="post" />
  </aside>
</template>

<script>
import SidebarPost from "./SidebarPost";

export default {
  name: "SidebarView",
  components: {
    SidebarPost
  },
  props: ["posts"]
};
</script>

<style scoped></style>
