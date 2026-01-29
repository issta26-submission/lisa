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
//<ID> 797
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *int_arr = NULL;
    cJSON *raw_item = NULL;
    char *printed = NULL;
    const char *version = NULL;
    int arr_size = 0;
    const int numbers[3] = {1, 2, 3};

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    int_arr = cJSON_CreateIntArray(numbers, 3);
    cJSON_AddItemToObject(root, "arr", int_arr);
    raw_item = cJSON_CreateRaw("{\"info\":\"raw\"}");
    cJSON_AddItemToObject(root, "meta", raw_item);
    version = cJSON_Version();
    printed = cJSON_PrintUnformatted(root);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(int_arr);
    (void)version;
    (void)arr_size;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}