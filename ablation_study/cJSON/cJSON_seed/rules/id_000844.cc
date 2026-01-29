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
//<ID> 844
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *greeting = NULL;
    cJSON *flag_true = NULL;
    cJSON *raw = NULL;
    cJSON *dup = NULL;
    char *printed = NULL;
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool is_raw = 0;
    cJSON_bool prealloc_ok = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    greeting = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(root, "greeting", greeting);
    flag_true = cJSON_AddTrueToObject(root, "active");
    raw = cJSON_CreateRaw("{\"meta\":42}");
    cJSON_AddItemToObject(root, "raw", raw);

    // step 3: Operate / Validate
    is_raw = cJSON_IsRaw(raw);
    dup = cJSON_Duplicate(root, 1);
    printed = cJSON_PrintUnformatted(dup);
    prealloc_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "greeting");
    cJSON_Delete(root);
    memset(buffer, 0, sizeof(buffer));

    // API sequence test completed successfully
    (void)is_raw;
    (void)prealloc_ok;
    (void)flag_true;
    return 66;
}