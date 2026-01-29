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
//<ID> 474
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(512);
    memset(buffer, 0, 512);
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON *item1 = cJSON_CreateString("first");
    cJSON *item2 = cJSON_CreateString("second");
    char *printed = NULL;
    cJSON *retr_items = NULL;
    cJSON *retr_title = NULL;
    int arr_size = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(items, item1);
    cJSON_AddItemToArray(items, item2);
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddStringToObject(root, "title", "demo_title");

    // step 3: Operate / Validate
    printed = cJSON_PrintBuffered(root, 256, 1);
    retr_items = cJSON_GetObjectItem(root, "items");
    arr_size = cJSON_GetArraySize(retr_items);
    retr_title = cJSON_GetObjectItem(root, "title");
    buffer[0] = (char)('0' + (arr_size % 10));
    buffer[1] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}