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
//<ID> 112
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(3.14159);
    cJSON *msg = cJSON_CreateString("world");
    cJSON *retrieved = NULL;
    cJSON *newnum = NULL;
    char *printed = NULL;
    double val = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "pi", num);
    cJSON_AddItemToObject(root, "greeting", msg);

    // step 3: Operate / Validate
    retrieved = cJSON_GetObjectItem(root, "pi");
    val = cJSON_GetNumberValue(retrieved);
    newnum = cJSON_CreateNumber(val * 2.0);
    cJSON_ReplaceItemViaPointer(root, retrieved, newnum);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}