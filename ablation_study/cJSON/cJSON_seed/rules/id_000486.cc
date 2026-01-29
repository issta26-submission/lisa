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
//<ID> 486
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    char *printed = NULL;
    cJSON *title_item = NULL;
    cJSON *flag_item = NULL;
    const char *title_str = NULL;
    cJSON_bool flag_true = 0;

    // step 2: Setup / Configure
    cJSON_AddStringToObject(meta, "greet", "hello");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(root, "title", "rootTitle");
    cJSON_AddTrueToObject(root, "active");

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    title_item = cJSON_GetObjectItem(root, "title");
    title_str = cJSON_GetStringValue(title_item);
    buffer[0] = title_str[0];
    flag_item = cJSON_GetObjectItem(root, "active");
    flag_true = cJSON_IsTrue(flag_item);
    buffer[1] = (char)('0' + (int)flag_true);
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}