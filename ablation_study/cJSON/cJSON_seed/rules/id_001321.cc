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
//<ID> 1321
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"arr\": [\"one\", \"two\"]}";
    const char *ver = NULL;
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *item0 = NULL;
    cJSON *dup_arr = NULL;
    cJSON *newobj = NULL;
    cJSON *countnum = NULL;
    cJSON *eqbool = NULL;
    int size = 0;
    cJSON_bool eq = 0;

    // step 2: Setup / Configure
    ver = cJSON_Version();
    root = cJSON_Parse(json);
    arr = cJSON_GetObjectItem(root, "arr");
    item0 = cJSON_GetArrayItem(arr, 0);
    dup_arr = cJSON_Duplicate(arr, 1);
    newobj = cJSON_CreateObject();
    cJSON_AddItemToObject(newobj, "dup_arr", dup_arr);
    cJSON_AddStringToObject(newobj, "first_value", cJSON_GetStringValue(item0));
    size = cJSON_GetArraySize(arr);
    countnum = cJSON_CreateNumber((double)size);
    cJSON_AddItemToObject(newobj, "count", countnum);
    eq = cJSON_Compare(arr, dup_arr, 1);
    eqbool = cJSON_CreateBool(eq);
    cJSON_AddItemToObject(newobj, "equal", eqbool);
    cJSON_AddStringToObject(newobj, "version", ver);

    // step 3: Operate / Validate
    (void)size;
    (void)eq;
    (void)item0;
    (void)ver;

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(newobj);
    // API sequence test completed successfully
    return 66;
}