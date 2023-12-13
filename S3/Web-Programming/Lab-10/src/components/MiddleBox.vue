<template>
  <div class="middle">
    <Sidebar :posts="viewPosts.slice(0, 2)" />
    <main>
      <Index v-if="page === 'Index'" :posts="viewPosts" />
      <Enter v-if="page === 'Enter'" />
      <WritePost v-if="page === 'WritePost'" />
      <EditPost v-if="page === 'EditPost'" />
      <Register v-if="page === 'Register'" />
      <Users v-if="page === 'Users'" :users="users" />
      <Post
        v-if="page === 'Post'"
        :post="currentPost"
        :comments="viewComments"
        :user="users[userId]"
      />
    </main>
  </div>
</template>

<script>
import SidebarBox from "./sidebar/SidebarBox";
import IndexPage from "./page/IndexPage";
import EnterPage from "./page/EnterPage";
import WritePostPage from "./page/WritePostPage";
import EditPostPage from "./page/EditPostPage";
import RegisterPage from "./page/RegisterPage";
import UsersPage from "./page/UsersPage";
import PostPage from "./page/PostPage";

export default {
  name: "MiddleBox",
  components: {
    WritePost: WritePostPage,
    Enter: EnterPage,
    Index: IndexPage,
    Sidebar: SidebarBox,
    EditPost: EditPostPage,
    Register: RegisterPage,
    Users: UsersPage,
    Post: PostPage
  },
  props: ["posts", "users", "comments", "userId"],
  data: function () {
    return {
      page: "Index",
      swap: {}
    };
  },
  computed: {
    viewPosts: function () {
      return Object.values(this.posts)
        .sort((a, b) => b.id - a.id)
        .map((post) => ({
          ...post,
          user: this.users[post.userId],
          commentCount: Object.values(this.comments).filter((x) => x.postId === post.id).length
        }));
    },
    currentPost: function () {
      const post = this.posts[this.swap];

      return {
        ...post,
        user: this.users[post.userId],
        commentCount: Object.values(this.comments).filter((x) => x.postId === post.id).length
      };
    },
    viewComments: function () {
      return Object.values(this.comments).map((comment) => ({
        ...comment,
        user: this.users[comment.userId]
      }));
    }
  },
  beforeCreate() {
    this.$root.$on("onChangePage", (page, swap) => {
      this.page = page;
      this.swap = swap;
    });
  }
};
</script>

<style scoped></style>
