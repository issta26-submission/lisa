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
//<ID> 1413
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json_text[] = "{\"name\": \"test\", \"values\": [1, 2, 3]}";
    size_t orig_len = sizeof(json_text) - 1;
    cJSON *parsed = NULL;
    cJSON *parsed_with_len = NULL;
    cJSON *injected = NULL;
    char *printed = NULL;
    char *minify_buf = NULL;
    size_t minified_len = 0;

    // step 2: Setup / Configure
    parsed = cJSON_Parse(json_text);
    injected = cJSON_CreateString("injected_value");
    cJSON_AddItemToObject(parsed, "injected", injected);

    // step 3: Operate / Validate
    printed = cJSON_PrintBuffered(parsed, 128, 1);
    minify_buf = (char *)cJSON_malloc(orig_len + 1);
    memset(minify_buf, 0, orig_len + 1);
    memcpy(minify_buf, json_text, orig_len + 1);
    cJSON_Minify(minify_buf);
    minified_len = (size_t)strlen(minify_buf);
    parsed_with_len = cJSON_ParseWithLength(minify_buf, minified_len);
    cJSON_free(printed);
    printed = cJSON_PrintBuffered(parsed_with_len, 64, 0);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(minify_buf);
    cJSON_Delete(parsed_with_len);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}