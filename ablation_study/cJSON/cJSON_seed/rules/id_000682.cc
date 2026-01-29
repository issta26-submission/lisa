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
//<ID> 682
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(512);
    memset(scratch, 0, 512);
    char *printed = NULL;
    const char *version = NULL;
    size_t printed_len = 0;
    size_t to_copy = 0;
    cJSON *root = cJSON_CreateObject();
    cJSON *child_created = cJSON_CreateObject();
    cJSON *child_added = NULL;
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON *checked = NULL;
    double read_value = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "child_created", child_created);
    child_added = cJSON_AddObjectToObject(root, "child_added");
    cJSON_AddItemToObject(child_added, "answer", num);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    to_copy = printed_len < 511 ? printed_len : 511;
    memcpy(scratch, printed ? printed : "", to_copy);
    scratch[to_copy] = '\0';
    cJSON_free(printed);
    printed = NULL;
    read_value = cJSON_GetNumberValue(cJSON_GetObjectItem(child_added, "answer"));
    checked = cJSON_CreateNumber(read_value);
    cJSON_AddItemToObject(root, "checked", checked);
    version = cJSON_Version();
    {
        size_t ver_len = version ? strlen(version) : 0;
        size_t max_copy = ver_len < (512 - 256 - 1) ? ver_len : (512 - 256 - 1);
        memcpy(scratch + 256, version ? version : "", max_copy);
        scratch[256 + max_copy] = '\0';
    }
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    to_copy = printed_len < 256 ? printed_len : 256;
    memcpy(scratch + 128, printed ? printed : "", to_copy);
    scratch[128 + to_copy] = '\0';
    cJSON_free(printed);
    printed = NULL;

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}