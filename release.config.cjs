module.exports = {
  branches: ['main'],
  plugins: [
    '@semantic-release/commit-analyzer',

    [
      '@semantic-release/release-notes-generator',
      {
        preset: 'conventionalcommits',
        writerOpts: {
          commitPartial:
            '- {{commitDate}} {{commitUrl}} {{#if scope}}**{{scope}}:** {{/if}}{{subject}}\n',

          commitGroupsSort: 'title',
          commitsSort: ['scope', 'subject'],
          groupBy: 'type',

          transform: function (commit, context) {
            // create a shallow clone of commit
            const c = Object.assign({}, commit);

            switch (commit.type) {
                case 'feat':
                    c.type = 'Features';
                    break;
                case 'fix':
                    c.type = 'Bug Fixes';
                    break;
                case 'perf':
                    c.type = 'Performance Improvements';
                    break;
                case 'docs':
                    c.type = 'Documentation';
                    break;
                case 'style':
                    c.type = 'Styles';
                    break;
                case 'refactor':
                    c.type = 'Code Refactoring';
                    break;
                case 'test':
                    c.type = 'Tests';
                    break;
                case 'chore':
                    c.type = 'Maintenance';
                    break;
                case 'ci':
                    c.type = 'Continuos Integration';
                    break;

                default:
                    return;
            }

            // clone references array
            if (Array.isArray(commit.references)) {
              c.references = commit.references.slice();
            }

            // shortHash formatted as 7 characters
            if (commit.hash) {
                c.shortHash = commit.hash.substring(0, 7);
            } else {
                c.shortHash = '';
            }

            // commitUrl formatted as Markdown
            if (c.shortHash) {
                c.commitUrl = `([${c.shortHash}](${context.host}/${context.owner}/${context.repository}/commit/${commit.hash}))`
            }

            // commitDate formatted as YYYY-MM-DD
            if (commit.committerDate) {
              const d = new Date(commit.committerDate);

              c.commitDate = isNaN(d.getTime()) ? '' : d.toISOString().slice(0, 10);
            } else {
              c.commitDate = '';
            }

            // header fallback
            c.header = commit.header || commit.subject || '';

            return c;
          }
        }
      }
    ],

    [
      '@semantic-release/github',
      {
        assets: [],
        failComment: false,
        labels: ['bug', 'maintenance'],
        releasedLabels: false,
        successComment: false
      }
    ]
  ]
};