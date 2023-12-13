<template>
  <div class="middle">
    <SidebarView :posts="viewPosts" />
    <main>
      <IndexView v-if="page === 'Index'" :posts="posts" />
      <EnterView v-if="page === 'Enter'" />
      <RegisterView v-if="page === 'Register'" />
      <UsersView v-if="page === 'Users'" />
      <PostView v-if="page === 'Post'" :post="swap" :full="true" :user="user" />
      <WriteView v-if="page === 'Write'" />
    </main>
  </div>
</template>

<script>
import SidebarView from "./sidebar/SidebarView";
import IndexView from "./page/IndexView";
import EnterView from "./page/EnterView";
import RegisterView from "./page/RegisterView";
import UsersView from "./page/UsersView";
import PostView from "./page/PostView";
import WriteView from "./page/WriteView";

export default {
  name: "MiddleView",
  components: {
    RegisterView,
    EnterView,
    IndexView,
    SidebarView,
    UsersView,
    PostView,
    WriteView
  },
  props: ["posts", "user"],
  data: function () {
    return {
      page: "Index",
      swap: undefined
    };
  },
  computed: {
    viewPosts: function () {
      return Object.values(this.posts)
        .sort((a, b) => b.id - a.id)
        .slice(0, 2);
    }
  },
  beforeCreate() {
    this.$root.$on("onChangePage", (page, swap) => {
      if (swap) {
        swap(this);
      }
      this.page = page;
    });
  }
};
</script>

<style scoped></style>
