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
//<ID> 1300
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    cJSON *item2 = NULL;
    cJSON *detached = NULL;
    const char *json = "{\"arr\":[true,false,true]}";
    cJSON_bool is0true = 0;
    cJSON_bool is2true = 0;
    char *printed = NULL;

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    arr = cJSON_GetObjectItem(root, "arr");

    // step 3: Operate / Validate
    item0 = cJSON_GetArrayItem(arr, 0);
    item1 = cJSON_GetArrayItem(arr, 1);
    item2 = cJSON_GetArrayItem(arr, 2);
    is0true = cJSON_IsTrue(item0);
    is2true = cJSON_IsTrue(item2);
    detached = cJSON_DetachItemViaPointer(arr, item1);
    printed = cJSON_PrintUnformatted(root);
    (void)is0true;
    (void)is2true;
    (void)printed;

    // step 4: Cleanup
    cJSON_Delete(detached);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}