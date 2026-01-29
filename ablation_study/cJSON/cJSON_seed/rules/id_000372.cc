#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 372
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *minified_buf = NULL;
    cJSON *root = cJSON_CreateObject();
    cJSON *parsed = NULL;
    cJSON *count_item = NULL;
    cJSON *new_count = NULL;
    double orig_count = 0.0;
    double replaced_count = 0.0;
    size_t printed_len = 0;

    // step 2: Setup / Configure
    cJSON_AddNumberToObject(root, "count", 7.0);
    cJSON_AddTrueToObject(root, "active");
    printed = cJSON_PrintUnformatted(root);
    printed_len = strlen(printed);
    minified_buf = (char *)cJSON_malloc(printed_len + 1);
    memcpy(minified_buf, printed, printed_len + 1);
    cJSON_Minify(minified_buf);
    parsed = cJSON_Parse(minified_buf);

    // step 3: Operate / Validate
    count_item = cJSON_GetObjectItem(parsed, "count");
    orig_count = cJSON_GetNumberValue(count_item);
    new_count = cJSON_CreateNumber(orig_count + 13.0);
    cJSON_ReplaceItemInObjectCaseSensitive(parsed, "count", new_count);
    count_item = cJSON_GetObjectItem(parsed, "count");
    replaced_count = cJSON_GetNumberValue(count_item);
    minified_buf[0] = (char)('0' + (int)replaced_count);
    minified_buf[1] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(minified_buf);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}