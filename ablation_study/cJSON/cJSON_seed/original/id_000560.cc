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
//<ID> 560
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *list = cJSON_CreateArray();
    cJSON *item0 = cJSON_CreateString("initial");
    cJSON_AddItemToArray(list, item0);
    cJSON_AddItemToObject(root, "list", list);
    cJSON_AddNumberToObject(root, "count", 1.0);
    cJSON *refs = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "refs", refs);
    cJSON_bool ref_added = cJSON_AddItemReferenceToArray(refs, item0);
    cJSON_AddNumberToObject(root, "ref_added", (double)ref_added);

    // step 2: Configure
    char *snapshot = cJSON_PrintBuffered(root, 128, 1);
    size_t snaplen = snapshot ? (strlen(snapshot) + 1) : 0;
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(snapshot, snaplen, &parse_end, 1);

    // step 3: Operate
    cJSON *parsed_list = cJSON_GetObjectItem(parsed, "list");
    cJSON *first = cJSON_GetArrayItem(parsed_list, 0);
    cJSON_SetValuestring(first, "modified");
    cJSON_bool has_refs = cJSON_HasObjectItem(parsed, "refs");
    cJSON_AddNumberToObject(parsed, "has_refs_flag", (double)has_refs);
    char *snapshot2 = cJSON_PrintBuffered(parsed, 64, 0);

    // step 4: Validate & Cleanup
    const char *extracted = cJSON_GetStringValue(cJSON_GetArrayItem(parsed_list, 0));
    (void)extracted;
    cJSON_free(snapshot);
    cJSON_free(snapshot2);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}