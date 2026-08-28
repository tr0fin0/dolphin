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
        assets: [
          {
            path: "hardware/projects/encoder/encoder.zip",
            name: "encoder-GER-${nextRelease.gitTag}.zip",
            label: "Encoder Gerber Files"
          },
          {
            path: "hardware/projects/encoder/encoder.step",
            name: "encoder-PCB-${nextRelease.gitTag}.step",
            label: "Encoder PCB STEP"
          },
          {
            path: "hardware/projects/encoder/encoder.pdf",
            name: "encoder-SCH-${nextRelease.gitTag}.pdf",
            label: "Encoder Schematic PDF"
          },
          {
            path: "hardware/projects/line/line.zip",
            name: "line-GER-${nextRelease.gitTag}.zip",
            label: "Line Sensor Gerber Files"
          },
          {
            path: "hardware/projects/line/line.step",
            name: "line-PCB-${nextRelease.gitTag}.step",
            label: "Line Sensor PCB STEP"
          },
          {
            path: "hardware/projects/line/line.pdf",
            name: "line-SCH-${nextRelease.gitTag}.pdf",
            label: "Line Sensor Schematic PDF"
          },
          {
            path: "hardware/projects/mainboard/mainboard.zip",
            name: "mainboard-GER-${nextRelease.gitTag}.zip",
            label: "Mainboard Gerber Files"
          },
          {
            path: "hardware/projects/mainboard/mainboard.step",
            name: "mainboard-PCB-${nextRelease.gitTag}.step",
            label: "Mainboard PCB STEP"
          },
          {
            path: "hardware/projects/mainboard/mainboard.pdf",
            name: "mainboard-SCH-${nextRelease.gitTag}.pdf",
            label: "Mainboard Schematic PDF"
          }
        ],
        failComment: false,
        labels: ['bug', 'maintenance'],
        releasedLabels: false,
        successComment: false
      }
    ]
  ]
};