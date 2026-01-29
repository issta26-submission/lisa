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
//<ID> 487
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *parsed = NULL;
    char *printed = NULL;
    cJSON *retrieved_meta = NULL;
    cJSON *title_item = NULL;
    cJSON *active_item = NULL;
    const char *title_str = NULL;
    int active_flag = 0;

    // step 2: Setup / Configure
    cJSON_AddStringToObject(root, "title", "ExampleTitle");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "greet", "hello");
    cJSON_AddTrueToObject(root, "active");
    cJSON_AddNumberToObject(root, "version", 1.0);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    retrieved_meta = cJSON_GetObjectItem(parsed, "meta");
    title_item = cJSON_GetObjectItem(parsed, "title");
    title_str = cJSON_GetStringValue(title_item);
    active_item = cJSON_GetObjectItem(parsed, "active");
    active_flag = (int)cJSON_IsTrue(active_item);
    buffer[0] = (char)('0' + active_flag);
    buffer[1] = '\0';
    (void)retrieved_meta;
    (void)title_str;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}