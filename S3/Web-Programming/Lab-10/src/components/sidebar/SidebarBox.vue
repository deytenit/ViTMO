<template>
  <aside>
    <SidebarPost v-for="post in posts" :key="post.id" :post="post" />
  </aside>
</template>

<script>
import SidebarPostBox from "./SidebarPostBox";

export default {
  name: "SidebarBox",
  components: {
    SidebarPost: SidebarPostBox
  },
  props: ["posts"]
};
</script>

<style scoped></style>
