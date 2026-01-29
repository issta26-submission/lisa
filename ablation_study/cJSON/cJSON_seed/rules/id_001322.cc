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
//<ID> 1322
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
    cJSON *s1 = NULL;
    cJSON *s2 = NULL;
    cJSON *s3 = NULL;
    cJSON *dupArr = NULL;
    const char *ver = NULL;
    int arr_size = 0;
    cJSON_bool eq = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "items");
    s1 = cJSON_CreateString("one");
    cJSON_AddItemToArray(arr, s1);
    s2 = cJSON_CreateString("two");
    cJSON_AddItemToArray(arr, s2);
    s3 = cJSON_CreateString("three");
    cJSON_AddItemToArray(arr, s3);
    ver = cJSON_Version();
    cJSON_AddStringToObject(root, "version", ver);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    dupArr = cJSON_Duplicate(arr, 1);
    eq = cJSON_Compare(arr, dupArr, 1);
    (void)arr_size;
    (void)eq;

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(dupArr);
    // API sequence test completed successfully
    return 66;
}