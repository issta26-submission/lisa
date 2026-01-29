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
//<ID> 968
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
    cJSON *n1 = NULL;
    cJSON *n2 = NULL;
    cJSON *n3 = NULL;
    cJSON *raw_item = NULL;
    char *printed = NULL;
    char *scratch = NULL;
    int arr_size = 0;
    double first_val = 0.0;
    const int scratch_len = 32;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "numbers");
    n1 = cJSON_CreateNumber(1.0);
    n2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    raw_item = cJSON_AddRawToObject(root, "meta", "{\"meta\":\"ok\",\"version\":1}");
    n3 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", n3);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    arr_size = cJSON_GetArraySize(cJSON_GetObjectItem(root, "numbers"));
    first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(cJSON_GetObjectItem(root, "numbers"), 0));
    scratch = (char *)cJSON_malloc((size_t)scratch_len);
    memset(scratch, 0, (size_t)scratch_len);
    scratch[0] = (char)('0' + (arr_size % 10));
    scratch[1] = (char)('A' + (((int)first_val) % 26));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}