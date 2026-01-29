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
//<ID> 1142
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *num = cJSON_CreateNumber(10.0);
    cJSON *str = cJSON_CreateString("hello");
    cJSON_AddItemToArray(items, num);
    cJSON_AddItemToArray(items, str);
    cJSON_AddStringToObject(meta, "author", "tester");

    // step 2: Configure
    cJSON *note = cJSON_CreateString("init");
    cJSON_AddItemToObject(root, "note", note);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(128);
    memset(scratch, 0, 128);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 3: Operate and Validate
    int size = cJSON_GetArraySize(items);
    cJSON *first = cJSON_GetArrayItem(items, 0);
    double first_val = cJSON_GetNumberValue(first);
    double computed_val = first_val + (double)size;
    cJSON *computed_item = cJSON_CreateNumber(computed_val);
    cJSON_AddItemToObject(root, "computed", computed_item);
    char *setret = cJSON_SetValuestring(note, "updated");
    cJSON *author_ref = cJSON_GetObjectItemCaseSensitive(meta, "author");
    char *author_val = cJSON_GetStringValue(author_ref);
    (void)setret;
    (void)author_val;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}