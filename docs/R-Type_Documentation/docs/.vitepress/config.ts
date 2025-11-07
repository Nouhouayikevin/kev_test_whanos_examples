import { defineConfig } from 'vitepress'

export default defineConfig({
  title: 'R-Type Documentation',
  description: 'Complete Documentation of the R-Type project',
  
  themeConfig: {
    nav: [
      { text: 'README', link: '/readme/' },
      { text: 'Developer Documentation', link: '/developer/' },
      { text: 'Technical & Comparative Study', link: '/study/' }
    ],

    sidebar: [
      {
        text: 'README',
        items: [
          { text: 'Readme', link: '/README' },
        ]
      },
      {
        text: 'Developer Documentation',
        items: [
          { text: 'Developer Documentation', link: '/DEVELOPER' },
        ]
      },
      {
        text: 'Technical & Comparative Study',
        items: [
          { text: 'Technical Choices', link: 'TECHNOLOGY_ANALYSIS' },
        ]
      }
    ]
  }
})