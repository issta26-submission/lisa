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
//<ID> 1325
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"arr\": [1, 2, 3], \"name\": \"root\"}";
    const char *version_str = NULL;
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *dup = NULL;
    cJSON_bool eq = 0;
    int arr_size = 0;
    char size_buf[32];
    char eq_buf[8];

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    arr = cJSON_GetObjectItem(root, "arr");
    arr_size = cJSON_GetArraySize(arr);
    version_str = cJSON_Version();
    cJSON_AddStringToObject(root, "cJSON_version", version_str);
    (void)version_str;

    // step 3: Operate / Validate
    snprintf(size_buf, sizeof(size_buf), "%d", arr_size);
    cJSON_AddStringToObject(root, "array_size", size_buf);
    dup = cJSON_Duplicate(root, 1);
    eq = cJSON_Compare(root, dup, 1);
    snprintf(eq_buf, sizeof(eq_buf), "%d", (int)eq);
    cJSON_AddStringToObject(root, "compare_equal", eq_buf);

    // step 4: Cleanup
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}